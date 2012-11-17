#ifndef ACTIONSLEEP_H
#define ACTIONSLEEP_H

#include "script/Action.h"

class RuleTimerThread;

class ActionSleep : public Action
{
public:
    ActionSleep();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void init(ConfigManager* config);
    void deinit();

    bool execute(unsigned int step);

    Type getType() const { return Sleep;}
    std::string getDescription() const;

    void setWaitMs(unsigned int waitMs) { m_waitMs = waitMs;}
    unsigned int getWaitMs() const { return m_waitMs;}

protected:
    RuleTimerThread* m_timerThread;
    unsigned int m_waitMs;
};

#endif // ACTIONSLEEP_H
