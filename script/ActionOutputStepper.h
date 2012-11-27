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
        SetParam = 3,
        EINVALID
    };

    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                 std::list<Rule::RequiredOutput>* listOutput,
                                 std::list<Rule::RequiredVariable>* listVariable) const;

    virtual StepperType getStepperType() const = 0;
};

#endif // ACTIONOUTPUTSTEPPER_H
