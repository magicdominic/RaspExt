
#include "hw/BTThread.h"
#include "util/Config.h"
#include "util/Debug.h"

#include <signal.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <poll.h>
#include <unistd.h>

#include <QDomDocument>

static void dummy_handler(int)
{
    // nothing here
}

BTThread::BTThread()
{
    m_bStop = false;
    m_thread = 0;
    m_socket = -1;

    // specify a dummy handler for SIGUSR1
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = dummy_handler;
    sigaction(SIGUSR1, &sa, NULL);
}

BTThread::~BTThread()
{
    if(m_thread != 0)
        this->kill();
}

void BTThread::start()
{
    pi_assert(m_thread == 0);

    m_bStop = false;

    // create and run thread
    pthread_create(&m_thread, NULL, BTThread::run_internal, (void*)this);
}

void BTThread::kill()
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

BTThread* BTThread::load(QDomElement* root)
{
    BTThread* btthread = new BTThread();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("name") == 0)
        {
            btthread->setName( elem.text().toStdString() );
        }
        else if(elem.tagName().toLower().compare("btaddress") == 0)
        {
            btthread->setBTAddr( elem.text().toStdString() );
        }

        elem = elem.nextSiblingElement();
    }

    if(btthread->getName().empty() || btthread->getBTAddr().empty())
    {
        pi_warn("Could not load bluetooth, name and/or address for this module were empty");
        delete btthread;
    }

    // start thread immediatly
    btthread->start();

    return btthread;
}

QDomElement BTThread::save(QDomElement* root, QDomDocument* document)
{
    QDomElement output = document->createElement("bluetooth");

    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode( QString::fromStdString( this->getName() ) );
    name.appendChild(nameText);

    output.appendChild(name);

    QDomElement btaddr = document->createElement("BTAddress");
    QDomText btaddrText = document->createTextNode( QString::fromStdString( this->getBTAddr() ) );
    btaddr.appendChild(btaddrText);

    output.appendChild(btaddr);

    root->appendChild(output);

    return output;
}

void BTThread::setBTAddr(std::string addr)
{
    // TODO: check format and only save if it is correct
    m_btaddr = addr;
}

void BTThread::run()
{
#ifdef USE_BLUETOOTH
    // connect to bluetooth board
    // try to connect until it succeeds
    int status;
    do
    {
        // TODO: we need the possibility to kill this thread here!

        // open socket
        struct sockaddr_l2 addr;

        m_socket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

        if(m_socket == -1)
        {
            pi_warn("Could not open bluetooth socket");
            return;
        }

        // set the connection parameters (who to connect to)
        addr.l2_family = AF_BLUETOOTH;
        addr.l2_psm = htobs(0x1001);
        addr.l2_cid = 0;
        str2ba(m_btaddr.c_str(), &addr.l2_bdaddr);

        // connect to target
        // maybe wo should do this in the run loop, as we may loose our connection to the board or it is not yet available
        status = connect(m_socket, (struct sockaddr*)&addr, sizeof(addr));
        if(status == -1)
        {
            perror("Could not connect to bt-board, retrying");

            // wait for some time before trying to reconnect
            sleep(1);

            // close and release all resoureces associated with m_socket, so that it can be reused
            close(m_socket);
        }
    }
    while(status == -1);


    // now we are connected and can enter the run loop

    timespec currentTime;
    timespec waitTime;
    while(true)
    {
        // Priorities:
        // 1. Incoming data
        // 2. Outputs
        // 3. Input Pollings


        // TODO: maybe this should be moved further down?

        struct pollfd fd;
        fd.fd = m_socket;
        fd.events = POLLIN;
        fd.revents = 0;

        // we have to check first if data is available, otherwise read would block
        int dataReady = poll(&fd, 1, 0);

        if(dataReady == 1)
        {
            // there is data ready to be read, so read it!
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            int readBytes = recv(m_socket, buffer, sizeof(buffer), 0);

            // we have to read data, now we have to parse the packet and call the apropriate functions
            printf("Received %d bytes: %s\n", readBytes, buffer);
        }


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
            //element.func(m_socket);
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

            // check if there is pending data to read, and if not wait for waitTime
            this->readWait(sleep);
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
            // calculate time difference for sleep
            waitTime = timespecSub(element.time, currentTime);

            if(this->readWait(waitTime))
            {
                // there is something to do before the timeout has expired, go back to start
                continue;
            }
        }

        // now we should do something as the timer has expired
        //element.hw->poll(m_handle);


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

    close(m_socket);
#endif
}

/**
 * @brief BTThread::waitForRead checks for pending data to read from the socket and if there is none waits for the amount of time specified by timeout
 * @param timeout specifies the amount of time to wait
 * @return true if there is pending data to read or it was interrupted by a signal (which most of the time means data to write), false if timeout has expired
 */
bool BTThread::readWait(timespec timeout)
{
    struct pollfd fd;
    fd.fd = m_socket;
    fd.events = POLLIN;
    fd.revents = 0;

    timespec local_timeout = timeout;
    int ret = ppoll(&fd, 1, &local_timeout, NULL);
    if(ret == -1)
    {
        // we have been interrupted by a signal
        pi_warn("Interrupted by signal");
        return true;
    }
    else if(ret == 0)
    {
        // timeout has expired
        return false;
    }
    else
    {
        // there is data to read
        return true;
    }
}

void* BTThread::run_internal(void* arg)
{
    BTThread* thread = (BTThread*)arg;
    thread->run();

    return NULL;
}
