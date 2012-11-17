#ifndef CONDITIONINPUTFADER_H
#define CONDITIONINPUTFADER_H

#include "script/ConditionInput.h"

class ConditionInputFader : public ConditionInput
{
public:
    enum Trigger
    {
        GreaterThan,
        LessThan,
        Equal,
        Changed,

        EINVALID,
    };
    static std::string TriggerToString(Trigger trigger);
    static Trigger StringToTrigger(std::string str);

    ConditionInputFader() { m_trigger = Equal; m_triggerValue = 100; m_isFulfilled = false;}

    static Condition* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void onInputChanged(HWInput *hw);
    void onInputDestroy(HWInput *hw);

    std::string getDescription() const;

    bool isFulfilled() const { return m_isFulfilled;}

    void setTrigger(Trigger trig) { m_isFulfilled = false; m_trigger = trig;}
    Trigger getTrigger() const { return m_trigger;}

    void setTriggerValue(unsigned int value) { m_triggerValue = value;}
    unsigned int getTriggerValue() const { return m_triggerValue;}

private:
    Trigger m_trigger;
    unsigned int m_triggerValue;

    bool m_isFulfilled;
};

#endif // CONDITIONINPUTFADER_H
