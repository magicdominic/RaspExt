#ifndef CONDITIONINPUT_H
#define CONDITIONINPUT_H

#include "script/Condition.h"
#include "hw/HWInputListener.h"

class ConditionInput : public Condition, public HWInputListener
{
public:
    static Condition* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    Type getType() const { return Input;}

    void init(ConfigManager* config);
    void deinit();

    virtual void onInputChanged(HWInput *hw) = 0;
    virtual void onInputDestroy(HWInput *hw) = 0;

    void setHWName(std::string str) { m_HWName = str;}
    std::string getHWName() const { return m_HWName;}

protected:
    std::string m_HWName;
    HWInput* m_hw;
};

#endif // CONDITIONINPUT_H
