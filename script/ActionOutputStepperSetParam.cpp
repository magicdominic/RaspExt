
#include "script/ActionOutputStepperSetParam.h"
#include "hw/HWOutputStepper.h"
#include "util/Debug.h"

ActionOutputStepperSetParam::ActionOutputStepperSetParam()
{
}

Action* ActionOutputStepperSetParam::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();
    ActionOutputStepperSetParam* action = new ActionOutputStepperSetParam();

    while(!elem.isNull())
    {
        elem = elem.nextSiblingElement();
    }

    return action;
}

QDomElement ActionOutputStepperSetParam::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = ActionOutputStepper::save(root, document);

    QDomElement steppertype = document->createElement("StepperType");
    QDomText steppertypeText = document->createTextNode("SetParam");
    steppertype.appendChild(steppertypeText);

    action.appendChild(steppertype);

    return action;
}

bool ActionOutputStepperSetParam::execute(unsigned int)
{
    pi_assert(m_hw != NULL);

    pi_warn("TODO");

    return true;
}

std::string ActionOutputStepperSetParam::getDescription() const
{
    std::string str = std::string("Set settings on ").append(m_HWName);

    return str;
}
