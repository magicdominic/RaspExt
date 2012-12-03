#ifndef RULE_H
#define RULE_H

#include <QDomElement>
#include <vector>

#include "hw/HWInput.h"
#include "hw/HWOutput.h"

class Condition;
class Action;
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

    /**
     * @brief The RequiredInput struct
     * This struct is used to get a list of required inputs from every Condition / Action.
     * Every input is specified using its name and its type. For correct operation both fields have to match the config.
     */
    struct RequiredInput
    {
        std::string name;
        HWInput::HWInputType type;
        bool exists;
    };

    /**
     * @brief The RequiredOutput struct
     * This struct is used to get a list of required ouputs from every Condition / Action.
     * Every outputs is specified using its name and its type. For correct operation both fields have to match the config.
     */
    struct RequiredOutput
    {
        std::string name;
        HWOutput::HWOutputType type;
        bool exists;
    };

    /**
     * @brief The RequiredVariable struct
     * This struct is used to get a list of required variables from every Condition / Action.
     * Every variables is matched using its name
     */
    struct RequiredVariable
    {
        std::string name;
        bool exists;
    };

    Rule();
    ~Rule();

    static Rule* load(QDomElement* root);
    void save(QDomElement* root, QDomDocument* document);

    // condition will be deleted by this class
    void addCondition(Condition* cond);
    void removeCondition(Condition* cond);
    // action will be deleted by this class
    void addAction(Action* action);

    void getRequiredList(std::list<RequiredInput>* listInput, std::list<RequiredOutput>* listOutput, std::list<RequiredVariable>* listVariable);

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
