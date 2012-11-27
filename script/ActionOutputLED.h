#ifndef ACTIONOUTPUTLED_H
#define ACTIONOUTPUTLED_H

#include "script/ActionOutput.h"

class ActionOutputLED : public ActionOutput
{
public:
    ActionOutputLED();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                 std::list<Rule::RequiredOutput>* listOutput,
                                 std::list<Rule::RequiredVariable>* listVariable) const;

    bool execute(unsigned int step);

    std::string getDescription() const;

    void setValue(unsigned int value) { m_value = value;}
    unsigned int getValue() const { return m_value;}

private:
    unsigned int m_value;

};

#endif // ACTIONOUTPUTLED_H
