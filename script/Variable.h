#ifndef VARIABLE_H
#define VARIABLE_H

#include <QDomElement>
#include <string>
#include <list>

class VariableListener;

class Variable
{
public:
    Variable();

    Variable* clone();

    static Variable* load(QDomElement* root);
    void save(QDomElement* root, QDomDocument* document);

    std::string getName() const { return m_name;}
    void setName(std::string str) { m_name = str;}

    int getDefaultValue() const { return m_defaultValue;}
    void setDefaultValue(int value) { m_defaultValue = value;}
    void setToDefault() { this->setValue(m_defaultValue);}
    int getValue() const { return m_value;}
    void setValue(int value);

    void registerVariableListener(VariableListener* listener); // registers an Object for this event, calls the Object immediatly after registration once
    void unregisterVariableListener(VariableListener* listener); // unregisters an Object
private:
    void variableChanged();

    std::string m_name;
    int m_value;
    int m_defaultValue;
    std::list<VariableListener*> m_listListeners;
};

#endif // VARIABLE_H
