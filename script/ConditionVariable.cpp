
#include "script/ConditionVariable.h"
#include "script/Variable.h"
#include "ConfigManager.h"
#include "util/Debug.h"

ConditionVariable::ConditionVariable()
{
    m_triggerValue = -1;
    m_trigger = Equal;
    m_isFulfilled = false;
}

Condition* ConditionVariable::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();

    ConditionVariable* condition = new ConditionVariable();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("name") == 0)
        {
            condition->setVarName( elem.text().toStdString() );
        }
        else if(elem.tagName().toLower().compare("value") == 0)
        {
            condition->setTriggerValue( elem.text().toInt() );
        }
        else if(elem.tagName().toLower().compare("trigger") == 0)
        {
            QString trigger = elem.text().toLower();
            if( trigger.compare("equal") == 0)
            {
                condition->setTrigger(Equal);
            }
            else if( trigger.compare("nolongerequal") == 0)
            {
                condition->setTrigger(NoLongerEqual);
            }
        }

        elem = elem.nextSiblingElement();
    }

    return condition;
}

QDomElement ConditionVariable::save(QDomElement* root, QDomDocument* document)
{
    QDomElement condition = Condition::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("variable");
    type.appendChild(typeText);

    condition.appendChild(type);

    // save name
    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode( QString::fromStdString(m_varName) );
    name.appendChild(nameText);

    condition.appendChild(name);

    QDomElement value = document->createElement("value");
    QDomText valueText = document->createTextNode( QString::number(m_triggerValue) );
    value.appendChild(valueText);

    condition.appendChild(value);

    // save trigger
    QDomElement trigger = document->createElement("trigger");
    QDomText triggerValue = document->createTextNode("");
    switch(m_trigger)
    {
    case Equal:
        triggerValue.setNodeValue("Equal");
        break;
    case NoLongerEqual:
        triggerValue.setNodeValue("NoLongerEqual");
        break;
    }

    trigger.appendChild(triggerValue);

    condition.appendChild(trigger);

    return condition;
}

void ConditionVariable::init(ConfigManager *config)
{
    m_var = config->getVariableByName(m_varName);

    pi_assert(m_var != NULL);

    if(m_var != NULL)
        m_var->registerVariableListener(this);
}

void ConditionVariable::deinit()
{
    if(m_var != NULL)
        m_var->unregisterVariableListener(this);
}

void ConditionVariable::onVariableChanged(Variable *var)
{
    pi_assert(m_var == var);

    bool isFulfilled = var->getValue() == m_triggerValue;

    if(m_trigger == NoLongerEqual)
        isFulfilled = !isFulfilled;

    if(m_isFulfilled != isFulfilled)
    {
        m_isFulfilled = isFulfilled;

        if(m_isFulfilled)
            this->conditionChanged();
    }
}

std::string ConditionVariable::getDescription() const
{
    std::string str = "If ";
    str.append( m_varName );
    if(m_trigger == Equal)
        str.append(" is equal to ");
    else // m_trigger == NoLongerEqual
        str.append(" is no longer equal to ");
    str.append( std::to_string(m_triggerValue) );

    return str;
}
