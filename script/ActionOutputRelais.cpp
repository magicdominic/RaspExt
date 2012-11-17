
#include "script/ActionOutputRelais.h"
#include "hw/HWOutputRelais.h"
#include "util/Debug.h"

ActionOutputRelais::ActionOutputRelais()
{
    m_state = Off;
}

Action* ActionOutputRelais::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();

    ActionOutputRelais* action = new ActionOutputRelais();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("state") == 0)
        {
            if(elem.text().toLower().compare("on") == 0)
                action->setState(On);
            else if(elem.text().toLower().compare("off") == 0)
                action->setState(Off);
            else if(elem.text().toLower().compare("toggle") == 0)
                action->setState(Toggle);
        }

        elem = elem.nextSiblingElement();
    }

    return action;
}

QDomElement ActionOutputRelais::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = ActionOutput::save(root, document);

    QDomElement subtype = document->createElement("subtype");
    QDomText subtypeText = document->createTextNode("Relais");
    subtype.appendChild(subtypeText);

    action.appendChild(subtype);

    QDomElement state = document->createElement("state");
    QDomText stateText = document->createTextNode("");

    switch(m_state)
    {
    case Off:
        stateText.setData("Off");
        break;
    case On:
        stateText.setData("On");
        break;
    case Toggle:
        stateText.setData("Toggle");
        break;
    }

    state.appendChild(stateText);

    action.appendChild(state);

    return action;
}

bool ActionOutputRelais::execute(unsigned int)
{
    pi_assert(m_hw != NULL);

    bool outputState = false;
    switch(m_state)
    {
    case On:
        outputState = true;
        break;
    case Off:
        outputState = false;
        break;
    case Toggle:
        outputState = !((HWOutputRelais*)m_hw)->getValue();
        break;
    }
    ((HWOutputRelais*)m_hw)->setValue(outputState);

    return true;
}

std::string ActionOutputRelais::getDescription() const
{
    std::string str = std::string("Set ").append(m_HWName).append(" to ");
    switch(m_state)
    {
    case On:
        str.append("ON");
        break;
    case Off:
        str.append("OFF");
        break;
    case Toggle:
        str.append("Toggle"); // TODO: find better description
        break;
    }

    return str;
}
