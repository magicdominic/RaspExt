#ifndef RULE_H
#define RULE_H

#include "script/Action.h"

#include <QDomElement>
#include <vector>

class Condition;
class ConfigManager;

class Rule
{
public:
    enum Type
    {
        Normal = 0,
        MustBeCalled = 1,
        EINVALID,
    };
    static Type StringToType(std::string str);
    static std::string TypeToString(Type type);

    Rule();
    ~Rule();

    static Rule* load(QDomElement* root);
    void save(QDomElement* root, QDomDocument* document);

    // condition will be deleted by this class
    void addCondition(Condition* cond);
    void removeCondition(Condition* cond);
    // action will be deleted by this class
    void addAction(Action* action);

    void conditionChanged(Condition* cond);

    void init(ConfigManager* config);
    void deinit();

    std::string getName() const { return m_name;}
    void setName(std::string str) { m_name = str;}

    void setType(Type type) { m_type = type;}
    Type getType() const { return m_type;}

    void call();

    // executeActions is used by RuleTimerThread, if one of the actions was Sleep
    void executeActions(unsigned int start = 0);

private:
    Type m_type;

    bool conditionsTrue();

    std::vector<Condition*> m_listConditions;
    std::vector<Action*> m_listActions;
    std::string m_name;

    friend class ActionTableModel;
    friend class ConditionTableModel;
};

#endif // RULE_H
