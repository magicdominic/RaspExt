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
    void deinit(ConfigManager* config);

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void testBemf();
    void setPosition(short position, bool override);
    void setDualPosition(short position1, short position2, unsigned char vmin, unsigned char vmax, bool override);
    void resetPosition(bool override);
    void softStop(bool override);
    void runVelocity(bool override);
    void setParam(Param param, bool override);
    void refreshFullStatus();
private:
    void poll(I2CThread* i2cThread);

    void testBemfI2C(I2CThread* i2cThread);
    void softStopI2C(I2CThread* i2cThread, bool override);
    void setPositionI2C(I2CThread* i2cThread, short position, bool override);
    void setDualPositionI2C(I2CThread* i2cThread, short position1, short position2, unsigned char vmin, unsigned char vmax, bool override);
    void resetPositionI2C(I2CThread* i2cThread, bool override);
    void runVelocityI2C(I2CThread* i2cThread, bool override);
    void setParamI2C(I2CThread* i2cThread, Param param, bool override);

    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTSTEPPERI2C_H
