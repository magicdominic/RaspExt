#ifndef ACTIONOUTPUTRELAY_H
#define ACTIONOUTPUTRELAY_H

#include "script/ActionOutput.h"

class ActionOutputRelay : public ActionOutput
{
public:
    enum State
    {
        Off = 0,
        On = 1,
        Toggle = 2,
    };

    ActionOutputRelay();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    bool execute(unsigned int step);

    std::string getDescription() const;

    void setState(State state) { m_state = state;}
    State getState() const { return m_state;}

private:
    State m_state;

};

#endif // ACTIONOUTPUTRELAY_H
