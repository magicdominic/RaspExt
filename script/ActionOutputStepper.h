#ifndef ACTIONOUTPUTSTEPPER_H
#define ACTIONOUTPUTSTEPPER_H

#include "script/ActionOutput.h"

class ActionOutputStepper : public ActionOutput
{
public:
    enum StepperType
    {
        SoftStop = 0,
        RunVelocity = 1,
        SetPosition = 2,
        SetStallParam = 3,
        EINVALID
    };

    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    virtual StepperType getStepperType() const = 0;
};

#endif // ACTIONOUTPUTSTEPPER_H
