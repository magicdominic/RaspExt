#ifndef HWOUTPUTDCMOTORI2C_H
#define HWOUTPUTDCMOTORI2C_H

#include "hw/HWOutputDCMotor.h"
#include "hw/I2COutput.h"

class I2CThread;

// this implementation is specific for DRV8830, it will not work for other dc motor drivers
class HWOutputDCMotorI2C : public HWOutputDCMotor, public I2COutput
{
public:
    HWOutputDCMotorI2C();

    virtual void init(ConfigManager* config);
    virtual void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void setI2C(int fd);
private:
    void outputChanged();

    int m_slaveAddress;
    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTDCMOTORI2C_H
