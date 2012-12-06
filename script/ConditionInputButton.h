#ifndef CONDITIONINPUTBUTTON_H
#define CONDITIONINPUTBUTTON_H

#include "script/ConditionInput.h"

class ConditionInputButton : public ConditionInput
{
public:
    enum Trigger
    {
        Pressed = 0,
        Released = 1,
        Changed = 2,

        EINVALID,
    };
    static std::string TriggerToString(Trigger trigger);
    static Trigger StringToTrigger(std::string str);

    ConditionInputButton() { m_trigger = Pressed; m_isFulfilled = false;}

    static Condition* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                 std::list<Rule::RequiredOutput>* listOutput,
                                 std::list<Rule::RequiredVariable>* listVariable) const;

    std::string getDescription() const;

    bool isFulfilled() const { return m_isFulfilled;}

    void setTrigger(Trigger trig) { m_isFulfilled = false; m_trigger = trig;}
    Trigger getTrigger() const { return m_trigger;}

private:
    void onInputChanged(HWInput *hw);

    Trigger m_trigger;
    bool m_isFulfilled;
};

#endif // CONDITIONINPUTBUTTON_H
