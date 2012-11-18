#ifndef ACTIONOUTPUTSTEPPERRUNVELOCITY_H
#define ACTIONOUTPUTSTEPPERRUNVELOCITY_H

#include "script/ActionOutputStepper.h"

class ActionOutputStepperRunVelocity : public ActionOutputStepper
{
public:
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    StepperType getStepperType() const { return RunVelocity;}

    bool execute(unsigned int step);
    std::string getDescription() const;
};

#endif // ACTIONOUTPUTSTEPPERRUNVELOCITY_H
