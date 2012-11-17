
#include "script/Rule.h"
#include "script/ConditionInput.h"
#include "util/Debug.h"

Rule::Rule()
{
    m_type = Normal;
}

Rule::~Rule()
{
    for(std::vector<Action*>::iterator it = m_listActions.begin(); it != m_listActions.end(); it++)
    {
        delete (*it);
    }

    for(std::vector<Condition*>::iterator it = m_listConditions.begin(); it != m_listConditions.end(); it++)
    {
        delete (*it);
    }
}

Rule* Rule::load(QDomElement* root)
{
    Rule* rule = new Rule();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("action") == 0)
        {
            Action* action = Action::load(&elem);
            if(action != NULL)
                rule->addAction(action);
        }
        else if(elem.tagName().toLower().compare("condition") == 0)
        {
            Condition* condition = Condition::load(&elem);
            if(condition != NULL)
                rule->addCondition(condition);
        }
        else if(elem.tagName().toLower().compare("name") == 0)
        {
            rule->setName( elem.text().toStdString() );
        }
        else if(elem.tagName().toLower().compare("type") == 0)
        {
            rule->setType( StringToType( elem.text().toStdString() ) );
        }

        elem = elem.nextSiblingElement();
    }

    return rule;
}

void Rule::save(QDomElement* root, QDomDocument* document)
{
    QDomElement rule = document->createElement("rule");

    for(std::vector<Condition*>::iterator it = m_listConditions.begin(); it != m_listConditions.end(); it++)
    {
        (*it)->save(&rule, document);
    }

    for(std::vector<Action*>::iterator it = m_listActions.begin(); it != m_listActions.end(); it++)
    {
        (*it)->save(&rule, document);
    }

    // save name
    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode(QString::fromStdString(this->m_name));
    name.appendChild(nameText);

    rule.appendChild(name);

    // save type
    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode( QString::fromStdString( TypeToString(m_type) ) );
    type.appendChild(typeText);

    rule.appendChild(type);

    root->appendChild(rule);
}

void Rule::addCondition(Condition *cond)
{
    cond->setRule(this);
    m_listConditions.push_back(cond);
}

void Rule::removeCondition(Condition* cond)
{
    for(std::vector<Condition*>::iterator it = m_listConditions.begin(); it != m_listConditions.end(); it++)
    {
        if(*it == cond)
        {
            m_listConditions.erase(it);
            break;
        }
    }
}

void Rule::addAction(Action *action)
{
    action->setRule(this);
    m_listActions.push_back(action);
}

void Rule::conditionChanged(Condition *cond)
{
    // only if type is normal, we react on changed conditions directly
    if(m_type != Normal)
        return;

    if(this->conditionsTrue())
    {
        this->executeActions();
    }
}

bool Rule::conditionsTrue()
{
    bool ret = true;
    for(std::vector<Condition*>::iterator it = m_listConditions.begin(); it != m_listConditions.end(); it++)
    {
        // maybe this can be optimized?
        ret = ret && (*it)->isFulfilled();
    }

    return ret;
}

void Rule::init(ConfigManager *config)
{
    // Actions and therefore Outputs have to be initialized before the conditions, because the inputs can fire as soon as they are initialized
    for(std::vector<Action*>::iterator it = m_listActions.begin(); it != m_listActions.end(); it++)
    {
        (*it)->init(config);
    }

    for(std::vector<Condition*>::iterator it = m_listConditions.begin(); it != m_listConditions.end(); it++)
    {
        (*it)->init(config);
    }
}

void Rule::deinit()
{
    // Conditions should be deinitialized before Actions, as they are the conditions for the actions (actually obvious)
    for(std::vector<Condition*>::iterator it = m_listConditions.begin(); it != m_listConditions.end(); it++)
    {
        (*it)->deinit();
    }

    for(std::vector<Action*>::iterator it = m_listActions.begin(); it != m_listActions.end(); it++)
    {
        (*it)->deinit();
    }
}

void Rule::executeActions(unsigned int start)
{
    pi_assert(start <= m_listActions.size());

    // start executing at start-element
    for(; start < m_listActions.size(); start++)
    {
        if( !(m_listActions.at(start)->execute(start) ) )
            break; // Action said we should stop executing other actions
    }
}

/**     call is used by ActionCallRule to call another rule.
 *      The conditions in this rule must be true, otherwise it is not going to be executed
 */
void Rule::call()
{
    pi_assert(m_type == MustBeCalled);

    if(this->conditionsTrue())
        this->executeActions();
}

Rule::Type Rule::StringToType(std::string str)
{
    const char* cstr = str.c_str();
    if( strcasecmp(cstr, "normal") == 0)
        return Normal;
    else if( strcasecmp(cstr, "mustbecalled") == 0)
        return MustBeCalled;
    else
        return EINVALID;
}

std::string Rule::TypeToString(Type type)
{
    switch(type)
    {
    case Normal:
        return "Normal";
        break;
    case MustBeCalled:
        return "MustBeCalled";
        break;
    default:
        pi_warn("Invalid type");
        return "";
        break;
    }
}
