#ifndef ACTIONOUTPUTRELAIS_H
#define ACTIONOUTPUTRELAIS_H

#include "script/ActionOutput.h"

class ActionOutputRelais : public ActionOutput
{
public:
    enum State
    {
        Off = 0,
        On = 1,
        Toggle = 2,
    };

    ActionOutputRelais();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    bool execute(unsigned int step);

    std::string getDescription() const;

    void setState(State state) { m_state = state;}
    State getState() const { return m_state;}

private:
    State m_state;

};

#endif // ACTIONOUTPUTRELAIS_H
