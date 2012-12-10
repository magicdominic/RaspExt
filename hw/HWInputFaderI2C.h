#ifndef HWINPUTFADERI2C_H
#define HWINPUTFADERI2C_H

#include "hw/HWInputFader.h"
#include "hw/I2CThread.h"

class HWInputFaderI2C : public HWInputFader, public I2CPolling
{
public:
    HWInputFaderI2C();

    virtual bool init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);
private:
    void poll(I2CThread* i2cThread);

    int m_slaveAddress;
    int m_channel;
};

#endif // HWINPUTFADERI2C_H
