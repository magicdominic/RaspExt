
#include "hw/I2CThread.h"
#include "hw/PCF8575I2C.h"

#include "util/Debug.h"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <signal.h>
#include <unistd.h>

#include "util/Config.h"

static void dummy_handler(int)
{
    // nothing here
}

I2CThread::I2CThread()
{
    // set m_handle to invalid value, so we can detect if we have to close it later
    m_handle = -1;
    m_bStop = false;
    m_thread = 0;

    // specify a dummy handler for SIGUSR1
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = dummy_handler;
    sigaction(SIGUSR1, &sa, NULL);

    // create and run thread
    pthread_create(&m_thread, NULL, I2CThread::run_internal, (void*)this);
}

I2CThread::~I2CThread()
{
    if(m_thread != 0)
        this->kill();
}

void I2CThread::kill()
{
    // set stop to true, so the thread should exit soon
    m_mutex.lock();
    m_bStop = true;
    m_mutex.unlock();

    // deliver signal to thread to wake it up
    pthread_kill(m_thread, SIGUSR1);

    pthread_join(m_thread, NULL);
    m_thread = 0;
}

void I2CThread::addInput(I2CPolling *hw, unsigned int freq)
{
    InputElement element;
    element.freq = freq;
    clock_gettime(CLOCK_MONOTONIC, &element.time);
    element.hw = hw;

    m_mutex.lock();
    m_inputQueue.push(element);
    m_mutex.unlock();

    // deliver signal to thread to wake it up
    // funny to use kill to wake a thread :-D
    pthread_kill(m_thread, SIGUSR1);
}

void I2CThread::removeInput(I2CPolling *hw)
{
    // we only use this element to remove the corresponding element from the queue
    InputElement element;
    element.hw = hw;

    m_mutex.lock();
    m_inputQueue.remove(element);
    m_mutex.unlock();
}

void I2CThread::addOutput(I2COutput *hw)
{
    OutputElement element;
    element.func = std::bind(&I2COutput::setI2C, hw, std::placeholders::_1);

    m_mutex.lock();
    m_outputQueue.push(element);
    m_mutex.unlock();
}

void I2CThread::addOutput(std::function<void (int)> func)
{
    OutputElement element;
    element.func = func;

    m_mutex.lock();
    m_outputQueue.push(element);
    m_mutex.unlock();
}

void I2CThread::addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port)
{
    // first check if we already have an Object for this slave address
    for(std::list<PCF8575I2C*>::iterator it = m_listPCF8575.begin(); it != m_listPCF8575.end(); it++)
    {
        if( (*it)->getSlaveAddress() == slaveAddress )
        {
            // we found it
            (*it)->addInput((HWInputButtonI2C*)hw, port);
            return;
        }
    }

    // we did not found an object for this slave address, so create a new one
    PCF8575I2C* pcf = new PCF8575I2C(slaveAddress);
    m_listPCF8575.push_back(pcf);

    pcf->init(this);

    pcf->addInput((HWInputButtonI2C*)hw, port);
}

void I2CThread::removeInputPCF8575(HWInput* hw, int slaveAddress)
{
    // search for the corresponding pcf object
    for(std::list<PCF8575I2C*>::iterator it = m_listPCF8575.begin(); it != m_listPCF8575.end(); it++)
    {
        if( (*it)->getSlaveAddress() == slaveAddress )
        {
            // we found it
            (*it)->removeInput((HWInputButtonI2C*)hw);

            // check if the pcf object is empty now
            if((*it)->empty())
            {
                (*it)->deinit();
                delete *it;

                m_listPCF8575.erase(it);
            }

            return;
        }
    }
}

PCF8575I2C* I2CThread::addOutputPCF8575(HWOutput* hw, int slaveAddress, unsigned int port)
{
    // first check if we already have an Object for this slave address
    for(std::list<PCF8575I2C*>::iterator it = m_listPCF8575.begin(); it != m_listPCF8575.end(); it++)
    {
        if( (*it)->getSlaveAddress() == slaveAddress )
        {
            // we found it
            (*it)->addOutput((HWOutputGPO*)hw, port);
            return (*it);
        }
    }

    // we did not found an object for this slave address, so create a new one
    PCF8575I2C* pcf = new PCF8575I2C(slaveAddress);
    m_listPCF8575.push_back(pcf);

    pcf->init(this);

    pcf->addOutput((HWOutputGPO*)hw, port);

    return pcf;
}

void I2CThread::removeOutputPCF8575(HWOutput* hw, int slaveAddress)
{
    // search for the corresponding pcf object
    for(std::list<PCF8575I2C*>::iterator it = m_listPCF8575.begin(); it != m_listPCF8575.end(); it++)
    {
        if( (*it)->getSlaveAddress() == slaveAddress )
        {
            // we found it
            (*it)->removeOutput((HWOutputGPO*)hw);

            // check if the pcf object is empty now
            if((*it)->empty())
            {
                (*it)->deinit();
                delete *it;

                m_listPCF8575.erase(it);
            }

            return;
        }
    }
}

void I2CThread::run()
{
#ifdef USE_I2C
    RPiRevision revision = getRPiRevision();

    switch(revision)
    {
    case Revision1:
        m_handle = open("/dev/i2c-0", O_RDWR);
        break;
    case Revision2:
        m_handle = open("/dev/i2c-1", O_RDWR);
        break;
    default:
        pi_warn("Unkown raspberry revision, aborting i2c");
        return;
    }

    if(m_handle < 0)
    {
        pi_warn("Could not open i2c-interface");
        return;
    }

    timespec currentTime;
    timespec waitTime;
    while(true)
    {
        // we should use an input and an output queue
        // the output queue has priority and commands in there are always handled first (starvation?)
        // after that the input queue

        m_mutex.lock();

        if(m_bStop)
        {
            // we want to exit the while loop
            m_mutex.unlock();
            break;
        }

        // if OutputQueue is not empty, then run the function in the element
        if( !m_outputQueue.empty() )
        {
            OutputElement element = m_outputQueue.front();
            m_outputQueue.pop();
            m_mutex.unlock();

            // run function
            element.func(m_handle);
            continue;
        }

        // check if inputQueue is empty, if yes then sleep
        if( m_inputQueue.empty() )
        {
            m_mutex.unlock();

            // sleep for 100ms and wake up if we receive a signal
            timespec sleep;
            sleep.tv_sec = 0;
            sleep.tv_nsec = 100000000;

            if(nanosleep(&sleep, NULL)  == -1)
            {
                // Interrupted by signal, normally this happens to wake this thread
            }
            continue;
        }

        // get element from inputQueue
        InputElement element = m_inputQueue.top();

        m_mutex.unlock();

        // get current time
        clock_gettime(CLOCK_MONOTONIC, &currentTime);

        // if this is true we have to sleep first
        if( timespecGreaterThan(element.time, currentTime) )
        {
            // calculate time difference for nanosleep
            waitTime = timespecSub(element.time, currentTime);

            // check for signals during sleep
            if( nanosleep(&waitTime, NULL) < 0)
            {
                // we have been interrupted by a signal, normally this happens to wake this thread

                // go back to start and check input and output queues
                continue;
            }
        }

        // now we should do something as the timer has expired
        element.hw->poll(m_handle);


        // preperation for next poll
        pi_assert(element.freq > 0);
        element.time.tv_nsec = currentTime.tv_nsec + 1000000000 / element.freq;
        if( element.time.tv_nsec > 1000000000 )
        {
            // we have to increase seconds by one
            element.time.tv_sec = currentTime.tv_sec + 1;
            element.time.tv_nsec = element.time.tv_nsec % 1000000000;
        }
        else
        {
            // otherwise just set seconds
            element.time.tv_sec = currentTime.tv_sec;
        }


        m_mutex.lock();

        // modified element will replace original one
        // If the element is already removed from the list, the following call does nothing
        m_inputQueue.modify(element);

        m_mutex.unlock();
    }

    close(m_handle);
#endif
}

void* I2CThread::run_internal(void* arg)
{
    I2CThread* thread = (I2CThread*)arg;
    thread->run();

    return NULL;
}
