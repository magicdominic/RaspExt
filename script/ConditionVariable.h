#ifndef CONDITIONVARIABLE_H
#define CONDITIONVARIABLE_H

#include "script/Condition.h"
#include "script/VariableListener.h"

class ConditionVariable : public Condition, public VariableListener
{
public:
    enum Trigger
    {
        Equal = 0,
        NoLongerEqual = 1,

        EINVALID
    };

    ConditionVariable();
    static Condition* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    virtual void getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                 std::list<Rule::RequiredOutput>* listOutput,
                                 std::list<Rule::RequiredVariable>* listVariable) const;

    void init(ConfigManager* config);
    void deinit();

    void onVariableChanged(Variable *var);

    void setVarName(std::string str) { m_varName = str;}
    std::string getVarName() const { return m_varName;}

    bool isFulfilled() const { return m_isFulfilled;}
    Type getType() const { return Var;}
    std::string getDescription() const;

    Trigger getTrigger() const { return m_trigger;}
    void setTrigger(Trigger trig) { m_isFulfilled = false; m_trigger = trig;}

    int getTriggerValue() const { return m_triggerValue;}
    void setTriggerValue(int value) { m_triggerValue = value;}

protected:
    Variable* m_var;
    Trigger m_trigger;
    int m_triggerValue;
    std::string m_varName;
    bool m_isFulfilled;
};

#endif // CONDITIONVARIABLE_H
