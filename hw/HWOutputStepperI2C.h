#ifndef HWOUTPUTSTEPPERI2C_H
#define HWOUTPUTSTEPPERI2C_H

#include "hw/HWOutputStepper.h"
#include "hw/I2CThread.h"

// this implementation is specific for AMIS 30624
class HWOutputStepperI2C : public HWOutputStepper, public I2CPolling
{
public:
    HWOutputStepperI2C();

    void init(ConfigManager* config);
    void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void poll(I2CThread* i2cThread);

    void testBemf();
    void setPosition(short position);
    void setDualPosition(short position1, short position2, unsigned char vmin, unsigned char vmax);
    void resetPosition();
    void softStop();
    void runVelocity();
    void refreshFullStatus();
    void setParam(Param param);
private:
    void testBemfI2C(I2CThread* i2cThread);
    void softStopI2C(I2CThread* i2cThread);
    void setPositionI2C(I2CThread* i2cThread, short position);
    void setDualPositionI2C(I2CThread* i2cThread, short position1, short position2, unsigned char vmin, unsigned char vmax);
    void resetPositionI2C(I2CThread* i2cThread);
    void runVelocityI2C(I2CThread* i2cThread);
    void setParamI2C(I2CThread* i2cThread, Param param);

    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTSTEPPERI2C_H
