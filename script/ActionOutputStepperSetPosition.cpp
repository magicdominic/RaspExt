
#include "script/ActionOutputStepperSetPosition.h"
#include "hw/HWOutputStepper.h"
#include "util/Debug.h"

ActionOutputStepperSetPosition::ActionOutputStepperSetPosition()
{
    m_position = 0;
}

Action* ActionOutputStepperSetPosition::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();
    ActionOutputStepperSetPosition* action = new ActionOutputStepperSetPosition();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("position") == 0)
        {
            action->setPosition( elem.text().toShort() );
        }

        elem = elem.nextSiblingElement();
    }

    return action;
}

QDomElement ActionOutputStepperSetPosition::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = ActionOutputStepper::save(root, document);

    QDomElement steppertype = document->createElement("StepperType");
    QDomText steppertypeText = document->createTextNode("SetPosition");
    steppertype.appendChild(steppertypeText);

    action.appendChild(steppertype);

    QDomElement position = document->createElement("Position");
    QDomText positionText = document->createTextNode( QString::number(m_position) );
    position.appendChild(positionText);

    action.appendChild(position);

    return action;
}

bool ActionOutputStepperSetPosition::execute(unsigned int)
{
    pi_assert(m_hw != NULL);

    ((HWOutputStepper*)m_hw)->setPosition(m_position);

    return true;
}

std::string ActionOutputStepperSetPosition::getDescription() const
{
    std::string str = std::string("Set ").append(m_HWName);
    str.append(" to target position ");
    str.append( std::to_string(m_position) );

    return str;
}
