
#include "hw/PCF8575I2C.h"
#include "hw/HWInputButtonI2C.h"
#include "hw/I2CThread.h"
#include "ConfigManager.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

PCF8575I2C::PCF8575I2C(int slaveAddress)
{
    m_slaveAddress = slaveAddress;
    m_portMask = 0;
    m_i2cThread = NULL;
}

void PCF8575I2C::addInput(HWInputButtonI2C *hw, unsigned int port)
{
    // TODO: check if already registered
    InputElement el;
    el.hw = hw;
    el.port = port;

    m_listInput.push_back( el );


    // Set port to 1, as only then if can detect inputs
    m_portMask = m_portMask | ( 1 << port );

    // we have to update I2C to set the new port mask
    this->updateI2C();
}

void PCF8575I2C::removeInput(HWInputButtonI2C *hw)
{
    for(std::list<InputElement>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        if(it->hw == hw)
        {
            // reset port mask, setting the bit to zero which this input corresponds to
            m_portMask = m_portMask & ~(1 << it->port);

            m_listInput.erase(it);
            break;
        }
    }
}

void PCF8575I2C::addOutput(HWInputButtonI2C *hw, unsigned int port)
{
    OutputElement el;
    el.hw = hw;
    el.port = port;

    m_listOutput.push_back(el);
}


void PCF8575I2C::setI2C(int fd)
{
    int ret;
    unsigned char buf[2];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }

    // directly write two bytes to the bus, these are the states of all inputs and outputs
    *((unsigned short*)buf) = m_portMask;
    ret = write(fd, buf, 2);
    if(ret != 2)
    {
        pi_warn("Could not write to bus");
        return;
    }
}


void PCF8575I2C::poll(int fd)
{
    int ret;
    unsigned char buf[2];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }

    // read back value
    ret = read(fd, buf, 2);
    if(ret != 2)
    {
        pi_warn("Could not read from bus");
        return;
    }

    unsigned short portState = *((unsigned short*)buf);

    for(std::list<InputElement>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        // call every HW Element to check, if its input has changed
        it->hw->onInputPolled((portState & (1 << it->port)) != 0 );
        // TODO: think about optimization here
    }
}

void PCF8575I2C::updateI2C()
{
    pi_assert(m_i2cThread != NULL);

    m_i2cThread->addOutput(this);
}

void PCF8575I2C::init(I2CThread* thread)
{
    m_i2cThread = thread;

    m_i2cThread->addInput(this, 50);
}

void PCF8575I2C::deinit()
{
    m_i2cThread->removeInput(this);
    m_i2cThread = NULL;
}
