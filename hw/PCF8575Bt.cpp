
#include "hw/PCF8575Bt.h"
#include "hw/HWInputButtonBt.h"
#include "hw/HWOutputGPO.h"
#include "hw/BTThread.h"
#include "ConfigManager.h"
#include "util/Debug.h"

PCF8575Bt::PCF8575Bt(int slaveAddress)
{
    m_slaveAddress = slaveAddress;
    m_portMask = 0;
    m_btThread = NULL;
}

void PCF8575Bt::addInput(HWInputButtonBt *hw, unsigned int port)
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

void PCF8575Bt::removeInput(HWInputButtonBt *hw)
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

void PCF8575Bt::addOutput(HWOutputGPO *hw, unsigned int port)
{
    OutputElement el;
    el.hw = hw;
    el.port = port;

    m_listOutput.push_back(el);

    hw->registerOutputListener(this);
}

void PCF8575Bt::removeOutput(HWOutputGPO *hw)
{
    for(std::list<OutputElement>::iterator it = m_listOutput.begin(); it != m_listOutput.end(); it++)
    {
        if(it->hw == hw)
        {
            hw->unregisterOutputListener(this);

            m_listOutput.erase(it);
            break;
        }
    }
}

void PCF8575Bt::setI2C(BTThread *btThread)
{
    BTI2CPacket packet;
    packet.read = 0;
    packet.request = 1;
    packet.slaveAddress = m_slaveAddress;

    packet.commandLength = 2;
    char* buf = (char*)malloc( packet.commandLength );
    *((unsigned short*)buf) = m_portMask;
    packet.commandBuffer = buf;

    btThread->sendI2CPackets(&packet, 1);
}


void PCF8575Bt::poll(BTThread* btThread)
{
    BTI2CPacket packet;
    packet.request = 1;
    packet.read = 1;
    packet.slaveAddress = m_slaveAddress;
    packet.commandLength = 0;
    packet.readLength = 2;

    btThread->sendI2CPackets(&packet, 1);
}

void PCF8575Bt::pollCallback(BTThread *btThread, BTI2CPacket *packet)
{
    // check if the packet is valid
    if(packet->read && !packet->request && !packet->error && packet->readLength == 2)
        return;

    unsigned short portState = *((unsigned short*)packet->readBuffer);

    for(std::list<InputElement>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        // call every HW Element to check, if its input has changed
        it->hw->onInputPolled( (portState & (1 << it->port)) == 0 );
        // TODO: think about optimization here
    }
}

void PCF8575Bt::updateI2C()
{
    pi_assert(m_btThread != NULL);

    m_btThread->addOutput( std::bind(&PCF8575Bt::setI2C, this, std::placeholders::_1) );
}

void PCF8575Bt::init(BTThread* btThread)
{
    m_btThread = btThread;

    m_btThread->addInput(this, 1); // TODO: change this
}

void PCF8575Bt::deinit()
{
    m_btThread->removeInput(this);
    m_btThread = NULL;
}

void PCF8575Bt::onOutputChanged(HWOutput *hw)
{/*
    HWOutputGPOI2C* hw_i2c = (HWOutputGPOI2C*)hw;

    unsigned int port =  hw_i2c->getPort();
    m_portMask = (m_portMask & ~(1 << port)) | (hw_i2c->getValue() ? 1 << port : 0);

    // after we have set the new port mask, we have to update the device as well
    this->updateI2C();*/
    // TODO
}
