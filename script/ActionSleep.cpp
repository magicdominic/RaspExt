
#include "script/ActionSleep.h"
#include "script/RuleTimerThread.h"
#include "ConfigManager.h"
#include "util/Debug.h"

ActionSleep::ActionSleep()
{
    m_timerThread = NULL;
    m_waitMs = 0;
}

Action* ActionSleep::load(QDomElement* root)
{
    ActionSleep* action = new ActionSleep();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("waitms") == 0)
        {
            action->setWaitMs( elem.text().toInt() );
        }

        elem = elem.nextSiblingElement();
    }

    if(action->getWaitMs() == 0)
    {
        delete action;
        return NULL;
    }

    return action;
}

QDomElement ActionSleep::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = Action::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("sleep");
    type.appendChild(typeText);

    action.appendChild(type);

    QDomElement wait = document->createElement("waitms");
    QDomText waitText = document->createTextNode( QString::number(m_waitMs) );
    wait.appendChild(waitText);

    action.appendChild(wait);

    return action;
}

void ActionSleep::init(ConfigManager *config)
{
    m_timerThread = config->getRuleTimerThread();

    pi_assert(m_timerThread != NULL);
}

void ActionSleep::deinit()
{
    m_timerThread = NULL;
}

bool ActionSleep::execute(unsigned int start)
{
    pi_assert(m_timerThread != NULL);

    m_timerThread->addRule(m_rule, start + 1, m_waitMs);

    return false;
}

std::string ActionSleep::getDescription() const
{
    std::string str = "Sleep for ";
    str.append( std::to_string(m_waitMs) );
    str.append(" ms");

    return str;
}
