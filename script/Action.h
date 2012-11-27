#ifndef ACTION_H
#define ACTION_H

#include "script/Rule.h"

#include <QDomElement>
#include <string>

class ConfigManager;

class Action
{
public:
    enum Type
    {
        Output = 0,
        Var = 1, // cannot be named Variable as this leads to conflict with the class
        Sleep = 2,
        CallRule = 3,
    };

    void setRule(Rule* rule) { m_rule = rule;}

    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    virtual void getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                 std::list<Rule::RequiredOutput>* listOutput,
                                 std::list<Rule::RequiredVariable>* listVariable) const {};

    virtual bool execute(__attribute__((unused)) unsigned int step) = 0; // return value specifies if Rule should continue executing
    virtual std::string getDescription() const = 0;
    virtual Type getType() const = 0;

    virtual void init(ConfigManager* config) = 0;
    virtual void deinit() = 0;

protected:
    Rule* m_rule;
};

#endif // ACTION_H
