#ifndef HWOUTPUTDCMOTORBT_H
#define HWOUTPUTDCMOTORBT_H

#include "hw/HWOutputDCMotor.h"

class BTThread;

// this implementation is specific for DRV8830, it will not work for other dc motor drivers
class HWOutputDCMotorBt : public HWOutputDCMotor
{
public:
    HWOutputDCMotorBt();

    HWType getHWType() const { return BtI2C;}

    virtual void init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    int getSlaveAddress() const { return m_slaveAddress;}
    void setSlaveAddress(int slaveAddress) { m_slaveAddress = slaveAddress;}

    std::string getBTName() const { return m_btName;}
    void setBTName(std::string name) { m_btName = name;}
private:
    void outputChanged();

    void setI2CBt(BTThread* btThread);

    int m_slaveAddress;
    std::string m_btName;
    BTThread* m_btThread;
};


#endif // HWOUTPUTDCMOTORBT_H
