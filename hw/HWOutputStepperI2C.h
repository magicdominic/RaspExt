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
    void setDualPosition(short position1, short position2, unsigned char vmin, unsigned char vmax);
    void resetPosition();
    void softStop();
    void runVelocity();
    void refreshFullStatus();
    void setParam(Param param);
private:
    void softStopI2C(int fd);
    void setPositionI2C(int fd, short position);
    void setDualPositionI2C(int fd, short position1, short position2, unsigned char vmin, unsigned char vmax);
    void resetPositionI2C(int fd);
    void runVelocityI2C(int fd);
    void setParamI2C(int fd, Param param);

    int m_slaveAddress;
    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTSTEPPERI2C_H
