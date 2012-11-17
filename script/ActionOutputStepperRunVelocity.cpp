
#include "script/ActionOutputStepperRunVelocity.h"
#include "hw/HWOutputStepper.h"
#include "util/Debug.h"

Action* ActionOutputStepperRunVelocity::load(QDomElement* root)
{
    return new ActionOutputStepperRunVelocity();
}

QDomElement ActionOutputStepperRunVelocity::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = ActionOutputStepper::save(root, document);

    QDomElement steppertype = document->createElement("StepperType");
    QDomText steppertypeText = document->createTextNode("RunVelocity");
    steppertype.appendChild(steppertypeText);

    action.appendChild(steppertype);

    return action;
}

bool ActionOutputStepperRunVelocity::execute(unsigned int)
{
    pi_assert(m_hw != NULL);

    ((HWOutputStepper*)m_hw)->runVelocity();

    return true;
}

std::string ActionOutputStepperRunVelocity::getDescription() const
{
    std::string str = std::string("Send command RunVelocity to ").append(m_HWName);

    return str;
}
