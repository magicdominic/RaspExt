#ifndef ACTIONOUTPUTGPO_H
#define ACTIONOUTPUTGPO_H

#include "script/ActionOutput.h"

class ActionOutputGPO : public ActionOutput
{
public:
    enum State
    {
        Low = 0,
        High = 1,
        Toggle = 2,
    };

    ActionOutputGPO();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                 std::list<Rule::RequiredOutput>* listOutput,
                                 std::list<Rule::RequiredVariable>* listVariable) const;

    bool execute(unsigned int step);

    std::string getDescription() const;

    void setState(State state) { m_state = state;}
    State getState() const { return m_state;}

private:
    State m_state;

};

#endif // ACTIONOUTPUTGPO_H
