
#include "hw/BLEThread.h"
#include "hw/HWInputButtonBtGPIO.h"
#include "util/Config.h"
#include "util/Debug.h"

#include <QDomDocument>

BLEThread::BLEThread()
{
}

BLEThread::~BLEThread()
{
}

void
BLEThread::start()
{

}

void
BLEThread::kill()
{

}

BTThread*
BLEThread::load(QDomElement *root)
{
    return new BLEThread();
}

QDomElement
BLEThread::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = BTThread::save(root, document);

    QDomElement lowEnergy = document->createElement("LowEnergy");

    output.appendChild(lowEnergy);

    return output;
}

void
BLEThread::addGPInput(HWInputButtonBtGPIO* hw)
{
    GPInput gp;
    gp.hw = hw;
    gp.pin = hw->getPin();
    gp.pinGroup = hw->getPinGroup();

    // we cannot handle other pins then those in pingroup 2 and only pins from 0 to 3
    if(gp.pinGroup != 2 || gp.pin > 3)
        return;

    m_listGPInput.push_back(gp);
}

void
BLEThread::removeGPInput(HWInputButtonBtGPIO* hw)
{
    for(std::list<GPInput>::iterator it = m_listGPInput.begin(); it != m_listGPInput.end(); it++)
    {
        if( (*it).hw == hw)
        {
            m_listGPInput.erase(it);
            break;
        }
    }
}




/*******************************************************
 * Virtual methods, not yet implemented
 *******************************************************/

void
BLEThread::addInput(BTI2CPolling* hw, unsigned int freq)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::removeInput(BTI2CPolling* hw)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::addOutput(std::function<void (BTThread*)> func)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::removeInputPCF8575(HWInput* hw, int slaveAddress)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::addOutputPCF8575(HWOutput* hw, int slaveAddress, unsigned int port)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::removeOutputPCF8575(HWOutput* hw, int slaveAddress)
{
    pi_warn("Not yet implemented");
}

// ATTENTION: USE ONLY IN BTTHREAD!!!!
void
BLEThread::sendI2CPackets(BTI2CPacket* packets, unsigned int num)
{
    pi_warn("Not yet implemented");
}
