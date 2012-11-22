#ifndef ACTIONOUTPUTSTEPPERSETPARAM_H
#define ACTIONOUTPUTSTEPPERSETPARAM_H

#include "script/ActionOutputStepper.h"

class ActionOutputStepperSetParam : public ActionOutputStepper
{
public:
    ActionOutputStepperSetParam();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    StepperType getStepperType() const { return SetParam;}

    bool execute(unsigned int step);
    std::string getDescription() const;
};

#endif // ACTIONOUTPUTSTEPPERSETPARAM_H
