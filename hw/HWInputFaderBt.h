#ifndef HWINPUTFADERBT_H
#define HWINPUTFADERBT_H

#include "hw/HWInputFader.h"
#include "hw/BTThread.h"

class HWInputFaderBt : public HWInputFader, public BTI2CPolling
{
public:
    HWInputFaderBt();

    HWType getHWType() const { return BtI2C;}

    virtual bool init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    int getChannel() const { return m_channel;}
    void setChannel(unsigned int channel) { m_channel = channel;}

    int getSlaveAddress() const { return m_slaveAddress;}
    void setSlaveAddress(int slaveAddress) { m_slaveAddress = slaveAddress;}

    std::string getBTName() const { return m_btName;}
    void setBTName(std::string name) { m_btName = name;}

private:
    void poll(BTThread* btThread);
    void onRead(BTThread* thread, BTI2CPacket* packet);

    int m_slaveAddress;
    int m_channel;
    std::string m_btName;

    BTThread* m_btThread;
};

#endif // HWINPUTFADERBT_H
