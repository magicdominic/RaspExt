#ifndef ACTIONCALLRULE_H
#define ACTIONCALLRULE_H

#include "script/Action.h"

class ActionCallRule : public Action
{
public:
    ActionCallRule();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void init(ConfigManager* config);
    void deinit();

    bool execute(unsigned int step);

    Type getType() const { return CallRule;}
    std::string getDescription() const;

    void setRuleName(std::string str) { m_ruleName = str;}
    std::string getRuleName() const { return m_ruleName;}

private:
    std::string m_ruleName;
    Rule* m_callRule;
};

#endif // ACTIONCALLRULE_H
