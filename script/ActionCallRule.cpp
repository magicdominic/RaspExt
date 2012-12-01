
#include "script/ActionCallRule.h"
#include "script/Script.h"
#include "ConfigManager.h"
#include "util/Debug.h"

ActionCallRule::ActionCallRule()
{
    m_callRule = NULL;
}

Action* ActionCallRule::load(QDomElement* root)
{
    ActionCallRule* action = new ActionCallRule();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("name") == 0)
        {
            action->setRuleName( elem.text().toStdString() );
        }

        elem = elem.nextSiblingElement();
    }

    if(action->getRuleName().empty())
    {
        delete action;
        return NULL;
    }

    return action;
}

QDomElement ActionCallRule::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = Action::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("CallRule");
    type.appendChild(typeText);

    action.appendChild(type);

    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode( QString::fromStdString( m_ruleName ) );
    name.appendChild(nameText);

    action.appendChild(name);

    return action;
}

void ActionCallRule::init(ConfigManager *config)
{
    Script* script = config->getActiveScript();

    pi_assert(script != NULL);

    m_rule = script->getRuleByName(m_ruleName);
}

void ActionCallRule::deinit()
{
    m_rule = NULL;
}

bool ActionCallRule::execute(unsigned int start)
{
    pi_assert(m_rule != NULL);

    m_rule->call();

    return true;
}

std::string ActionCallRule::getDescription() const
{
    std::string str = "Call rule ";
    str.append( m_ruleName );

    return str;
}
