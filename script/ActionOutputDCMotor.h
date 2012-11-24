#ifndef ACTIONOUTPUTDCMOTOR_H
#define ACTIONOUTPUTDCMOTOR_H

#include "script/ActionOutput.h"
#include "hw/HWOutputDCMotor.h"

class HWInput;

class ActionOutputDCMotor : public ActionOutput
{
public:
    ActionOutputDCMotor();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void init(ConfigManager* config);
    void deinit();

    bool execute(unsigned int step);

    std::string getDescription() const;

    void setSpeed(unsigned int speed) { m_speed = speed;}
    unsigned int getSpeed() const { return m_speed;}

    void setInputName(std::string str) { m_inputName = str;}
    std::string getInputName() const { return m_inputName;}

    void setState(HWOutputDCMotor::MotorState state) { m_state = state;}
    HWOutputDCMotor::MotorState getState() const { return m_state;}
private:
    unsigned int m_speed;
    HWOutputDCMotor::MotorState m_state;
    std::string m_inputName;
    HWInput* m_hwInput;
};

#endif // ACTIONOUTPUTDCMOTOR_H
