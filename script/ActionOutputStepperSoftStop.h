#ifndef ACTIONOUTPUTSTEPPERSOFTSTOP_H
#define ACTIONOUTPUTSTEPPERSOFTSTOP_H

#include "script/ActionOutputStepper.h"

class ActionOutputStepperSoftStop : public ActionOutputStepper
{
public:
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    StepperType getStepperType() const { return SoftStop;}

    bool execute(unsigned int step);
    std::string getDescription() const;
};

#endif // ACTIONOUTPUTSTEPPERSOFTSTOP_H
