#ifndef HWINPUTFADERI2C_H
#define HWINPUTFADERI2C_H

#include "hw/HWInputFader.h"
#include "hw/I2CThread.h"

class HWInputFaderI2C : public HWInputFader, public I2CPolling
{
public:
    HWInputFaderI2C();

    HWType getHWType() const { return I2C;}

    virtual bool init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    int getChannel() const { return m_channel;}
    void setChannel(unsigned int channel) { m_channel = channel;}

    int getSlaveAddress() const { return m_slaveAddress;}
    void setSlaveAddress(int slaveAddress) { m_slaveAddress = slaveAddress;}
private:
    void poll(I2CThread* i2cThread);

    int m_slaveAddress;
    int m_channel;
};

#endif // HWINPUTFADERI2C_H
