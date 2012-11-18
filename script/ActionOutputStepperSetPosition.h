#ifndef ACTIONOUTPUTSTEPPERPOSITION_H
#define ACTIONOUTPUTSTEPPERPOSITION_H

#include "script/ActionOutputStepper.h"

class ActionOutputStepperSetPosition : public ActionOutputStepper
{
public:
    ActionOutputStepperSetPosition();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    StepperType getStepperType() const { return SetPosition;}

    bool execute(unsigned int step);
    std::string getDescription() const;

    void setPosition(short position) { m_position = position;}
    short getPosition() const { return m_position;}

private:
    short m_position;
};

#endif // ACTIONOUTPUTSTEPPERPOSITION_H
