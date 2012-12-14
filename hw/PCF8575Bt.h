#ifndef PCF8575BT_H
#define PCF8575BT_H

#include "hw/HWOutputListener.h"
#include "hw/BTThread.h"

#include <list>

class HWInputButtonBt;
class HWOutputGPO;

class PCF8575Bt : public BTI2CPolling, HWOutputListener
{
public:
    PCF8575Bt(int slaveAddress);

    void addInput(HWInputButtonBt* hw, unsigned int port);
    void removeInput(HWInputButtonBt* hw);

    void addOutput(HWOutputGPO* hw, unsigned int port);
    void removeOutput(HWOutputGPO* hw);

    bool empty() const { return m_listInput.empty() && m_listOutput.empty();}

    int getSlaveAddress() const { return m_slaveAddress;}


    void init(BTThread* btThread);
    void deinit();

private:
    void poll(BTThread* btThread);
    void pollCallback(BTThread* btThread, BTI2CPacket* packet);
    void setI2C(BTThread* btThread);

    void onOutputChanged(HWOutput *hw);

    struct InputElement
    {
        unsigned int port;
        HWInputButtonBt* hw;
    };

    struct OutputElement
    {
        unsigned int port;
        HWOutputGPO* hw;
    };

    void updateI2C();

    BTThread* m_btThread;

    unsigned short m_portMask;
    int m_slaveAddress;

    std::list<InputElement> m_listInput;
    std::list<OutputElement> m_listOutput;
};


#endif // PCF8575BT_H
