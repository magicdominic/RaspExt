
#include "script/Variable.h"
#include "script/VariableListener.h"
#include "util/Debug.h"

Variable::Variable()
{
    m_value = 0;
    m_defaultValue = 0;
}

/**
 * @brief Variable::clone This method clones this object. If there are listeners registered, this function cannot be used as this is dangerous
 * @return clone of the given object
 */
Variable* Variable::clone()
{
    pi_assert(m_listListeners.empty());

    Variable* var = new Variable();
    var->m_name = m_name;
    var->m_value = m_value;
    var->m_defaultValue = m_defaultValue;

    return var;
}

Variable* Variable::load(QDomElement* root)
{
    Variable* variable = new Variable();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("defaultvalue") == 0)
        {
            variable->m_defaultValue = elem.text().toInt();
            variable->setValue( variable->m_defaultValue );
        }
        else if(elem.tagName().toLower().compare("name") == 0)
        {
            variable->m_name = elem.text().toStdString();
        }

        elem = elem.nextSiblingElement();
    }

    return variable;
}

void Variable::save(QDomElement* root, QDomDocument* document)
{
    QDomElement var = document->createElement("variable");
    // save name
    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode(QString::fromStdString( m_name ));
    name.appendChild(nameText);

    var.appendChild(name);

    QDomElement value = document->createElement("defaultValue");
    QDomText valueText = document->createTextNode( QString::number(m_defaultValue) );
    value.appendChild(valueText);

    var.appendChild(value);

    root->appendChild(var);
}

void Variable::setValue(int value)
{
    if(value != m_value)
    {
        m_value = value;
        this->variableChanged();
    }
}

void Variable::variableChanged()
{
    for(std::list<VariableListener*>::iterator it = m_listListeners.begin(); it != m_listListeners.end(); it++)
    {
        (*it)->onVariableChanged(this);
    }
}

/**
 * @brief Variable::registerVariableListener registers the object listener for the onVariableChanged event.
 * This method calls the event handler immediately once after registration
 * @param listener
 */
void Variable::registerVariableListener(VariableListener *listener)
{
    // TODO: check if already registered

    m_listListeners.push_back(listener);

    listener->onVariableChanged(this);
}

/**
 * @brief Variable::unregisterVariableListener unregisters an object for the onVariableChanged event.
 * @param listener
 */
void Variable::unregisterVariableListener(VariableListener *listener)
{
    // TODO: check if registered, otherwise raise error

    m_listListeners.remove(listener);
}
