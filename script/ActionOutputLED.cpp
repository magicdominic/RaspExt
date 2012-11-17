
#include "script/ActionOutputLED.h"
#include "hw/HWOutputLED.h"
#include "util/Debug.h"

ActionOutputLED::ActionOutputLED()
{
    m_value = 0;
}

Action* ActionOutputLED::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();

    ActionOutputLED* action = new ActionOutputLED();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("value") == 0)
        {
            action->m_value = elem.text().toInt();
        }

        elem = elem.nextSiblingElement();
    }

    return action;
}

QDomElement ActionOutputLED::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = ActionOutput::save(root, document);

    QDomElement subtype = document->createElement("subtype");
    QDomText subtypeText = document->createTextNode("LED");
    subtype.appendChild(subtypeText);

    action.appendChild(subtype);

    QDomElement value = document->createElement("value");
    QDomText valueText = document->createTextNode( QString::number( m_value ) );
    value.appendChild(valueText);

    action.appendChild(subtype);

    return action;
}

bool ActionOutputLED::execute(unsigned int)
{
    pi_assert(m_hw != NULL);

    ((HWOutputLED*)m_hw)->setValue( m_value );

    return true;
}

std::string ActionOutputLED::getDescription() const
{
    std::string str = std::string("Set ").append(m_HWName).append(" to ");
    str.append( std::to_string( m_value ) );

    return str;
}
