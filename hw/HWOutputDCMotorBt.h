#ifndef HWOUTPUTDCMOTORBT_H
#define HWOUTPUTDCMOTORBT_H

#include "hw/HWOutputDCMotor.h"
#include "hw/I2COutput.h"

class BTThread;

// this implementation is specific for DRV8830, it will not work for other dc motor drivers
class HWOutputDCMotorBt : public HWOutputDCMotor
{
public:
    HWOutputDCMotorBt();

    virtual void init(ConfigManager* config);
    virtual void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);
private:
    void outputChanged();

    void setI2CBt(BTThread* btThread);

    int m_slaveAddress;
    std::string m_btName;
    BTThread* m_btThread;
};


#endif // HWOUTPUTDCMOTORBT_H
