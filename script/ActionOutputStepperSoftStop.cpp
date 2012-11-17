
#include "script/ActionOutputStepperSoftStop.h"
#include "hw/HWOutputStepper.h"
#include "util/Debug.h"

Action* ActionOutputStepperSoftStop::load(QDomElement* root)
{
    return new ActionOutputStepperSoftStop();
}

QDomElement ActionOutputStepperSoftStop::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = ActionOutputStepper::save(root, document);

    QDomElement steppertype = document->createElement("StepperType");
    QDomText steppertypeText = document->createTextNode("SoftStop");
    steppertype.appendChild(steppertypeText);

    action.appendChild(steppertype);

    return action;
}

bool ActionOutputStepperSoftStop::execute(unsigned int)
{
    pi_assert(m_hw != NULL);

    ((HWOutputStepper*)m_hw)->softStop();

    return true;
}

std::string ActionOutputStepperSoftStop::getDescription() const
{
    std::string str = std::string("Send command soft stop to ").append(m_HWName);

    return str;
}
