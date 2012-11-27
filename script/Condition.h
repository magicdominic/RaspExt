#ifndef CONDITION_H
#define CONDITION_H

#include "script/Rule.h"

#include <string>

class Condition
{
public:
    enum Type
    {
        Input = 0,
        Var = 1, // cannot be named Variable as this leads to conflict with the class
    };

    void setRule(Rule* rule) { m_rule = rule;}

    static Condition* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    virtual void getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                 std::list<Rule::RequiredOutput>* listOutput,
                                 std::list<Rule::RequiredVariable>* listVariable) const {};

    virtual void init(ConfigManager* config) = 0;
    virtual void deinit() = 0;

    virtual bool isFulfilled() const = 0;
    virtual Type getType() const = 0;
    virtual std::string getDescription() const = 0;

protected:
    void conditionChanged() { m_rule->conditionChanged(this);}

private:
    Rule* m_rule;
};

#endif // CONDITION_H
