
#include "script/Variable.h"
#include "script/VariableListener.h"

Variable::Variable()
{
    m_value = 0;
    m_defaultValue = 0;
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

void Variable::registerVariableListener(VariableListener *listener)
{
    // TODO: check if already registered

    m_listListeners.push_back(listener);

    listener->onVariableChanged(this);
}

void Variable::unregisterVariableListener(VariableListener *listener)
{
    // TODO: check if registered, otherwise raise error

    m_listListeners.remove(listener);
}
