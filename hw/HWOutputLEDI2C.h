#ifndef HWOUTPUTLEDI2C_H
#define HWOUTPUTLEDI2C_H

#include "hw/HWOutputLED.h"

class I2CThread;

// this implementation is specific for TLC59116, it will not work for other led drivers
class HWOutputLEDI2C : public HWOutputLED
{
public:
    HWOutputLEDI2C();

    HWType getHWType() const { return I2C;}

    virtual void init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    int getChannel() const { return m_channel;}
    void setChannel(unsigned int channel) { m_channel = channel;}

    int getSlaveAddress() const { return m_slaveAddress;}
    void setSlaveAddress(int slaveAddress) { m_slaveAddress = slaveAddress;}
private:
    void outputChanged();

    void setI2C(I2CThread *i2cThread);
    void setupI2C(I2CThread *i2cThread);

    int m_slaveAddress;
    unsigned int m_channel;
    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTLEDI2C_H
