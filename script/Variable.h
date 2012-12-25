#ifndef VARIABLE_H
#define VARIABLE_H

#include <QDomElement>
#include <string>
#include <list>

class VariableListener;

/**
 * @brief The Variable class is special type of input and output, as it can be used as both.
 * A variable has an integer value which can be manipulated by actions and used as conditions.
 */
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

    void registerVariableListener(VariableListener* listener);
    void unregisterVariableListener(VariableListener* listener);
private:
    void variableChanged();

    std::string m_name;
    int m_value;
    int m_defaultValue;
    std::list<VariableListener*> m_listListeners;
};

#endif // VARIABLE_H
