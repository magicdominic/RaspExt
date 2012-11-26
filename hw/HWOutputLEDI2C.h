#ifndef HWOUTPUTLEDI2C_H
#define HWOUTPUTLEDI2C_H

#include "hw/HWOutputLED.h"
#include "hw/I2COutput.h"

class I2CThread;

// this implementation is specific for TLC59116, it will not work for other led drivers
class HWOutputLEDI2C : public HWOutputLED, public I2COutput
{
public:
    HWOutputLEDI2C();

    virtual void init(ConfigManager* config);
    virtual void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void setI2C(int fd);
private:
    void outputChanged();

    void setupI2C(int fd);

    int m_slaveAddress;
    unsigned int m_channel;
    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTLEDI2C_H
