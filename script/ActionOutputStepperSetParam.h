#ifndef ACTIONOUTPUTSTEPPERSETPARAM_H
#define ACTIONOUTPUTSTEPPERSETPARAM_H

#include "script/ActionOutputStepper.h"
#include "hw/HWOutputStepper.h"

class ActionOutputStepperSetParam : public ActionOutputStepper
{
public:
    ActionOutputStepperSetParam();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    StepperType getStepperType() const { return SetParam;}

    bool execute(unsigned int step);
    std::string getDescription() const;

    HWOutputStepper::Param getParam() const { return m_param;}
    void setParam(HWOutputStepper::Param param) { m_param = param;}

private:
    HWOutputStepper::Param m_param;
};

#endif // ACTIONOUTPUTSTEPPERSETPARAM_H
