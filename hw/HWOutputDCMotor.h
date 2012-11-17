#ifndef HWOUTPUTDCMOTOR_H
#define HWOUTPUTDCMOTOR_H

#include "hw/HWOutput.h"

class HWOutputDCMotor : public HWOutput
{
public:
    // MotorState is used in I2C Implementation, DO NOT CHANGE
    enum MotorState
    {
        Standby = 0,
        Reverse = 1,
        Forward = 2,
        Brake = 3,
        EINVALID
    };

    HWOutputDCMotor();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void setOverrideMotorState(MotorState state);
    MotorState getMotorState() const { return m_state;}
    void setOverrideSpeed(unsigned int speed);
    unsigned int getSpeed() const { return m_speed;}

    void setOverrideSpeedAndState(unsigned int speed, MotorState state);
    void setSpeedAndState(unsigned int speed, MotorState state);

    static std::string MotorStateToString(MotorState type);
    static MotorState StringToMotorState(std::string str);

protected:
    MotorState m_state;
    unsigned int m_speed; // 0 to 100 percent
};

#endif // HWOUTPUTDCMOTOR_H
