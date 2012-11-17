#ifndef ACTIONVARIABLE_H
#define ACTIONVARIABLE_H

#include "script/Action.h"
#include "script/Variable.h"

class ActionVariable : public Action
{
public:
    enum Operator
    {
        Equal = 0,
        Plus = 1,
        Minus = 2,
        EINVALID
    };

    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void setVarName(std::string str) { m_varName = str;}
    std::string getVarName() const { return m_varName;}

    void init(ConfigManager* config);
    void deinit();

    bool execute(unsigned int step);

    Type getType() const { return Var;}
    std::string getDescription() const;

    void setOperator(Operator op) { m_operator = op;}
    Operator getOperator() const { return m_operator;}

    void setOperand(int value) { m_operand = value;}
    int getOperand() const { return m_operand;}

    static std::string OperatorToString(Operator op);
    static Operator StringToOperator(std::string str);
protected:
    std::string m_varName;
    int m_operand;
    Operator m_operator;
    Variable* m_var;
};

#endif // ACTIONVARIABLE_H
