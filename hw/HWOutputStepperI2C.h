#ifndef HWOUTPUTSTEPPERI2C_H
#define HWOUTPUTSTEPPERI2C_H

#include "hw/HWOutputStepper.h"
#include "hw/I2CPolling.h"

class I2CThread;

// this implementation is specific for AMIS 30624
class HWOutputStepperI2C : public HWOutputStepper, public I2CPolling
{
public:
    HWOutputStepperI2C();

    void init(ConfigManager* config);
    void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void poll(int fd);

    void setPosition(short position);
    void softStop();
    void runVelocity();
    void refreshFullStatus();
private:
    void softStopI2C(int fd);
    void setPositionI2C(int fd);
    void runVelocityI2C(int fd);

    int m_slaveAddress;
    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTSTEPPERI2C_H
