#ifndef CONDITIONDIALOG_H
#define CONDITIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>

#include "script/Rule.h"
#include "script/Condition.h"

namespace Ui {
    class ConditionDialog;
}

class Script;
class IConditionWidget;

class ConditionDialog : public QDialog
{
    Q_OBJECT
public:
    ConditionDialog(QWidget *parent, Script* script);
    ~ConditionDialog();

    void edit(Condition* condition);

    Condition* getCondition() const { return m_condition;}

private slots:
    void okPressed();
    void conditionChanged(int index);

private:
    Ui::ConditionDialog* ui;
    Condition* m_condition;
    Script* m_script;

    IConditionWidget* m_baseWidget;
};

class IConditionWidget : public QWidget
{
public:
    IConditionWidget(QWidget* parent) : QWidget(parent) {};
    virtual Condition* assemble() = 0;
    virtual void edit(Condition* condition) = 0;
};

// Input Stuff
class ConditionInputWidget : public IConditionWidget
{
    Q_OBJECT
public:
    ConditionInputWidget(QWidget* parent, Script* script);
    void edit(Condition* condition);

    Condition* assemble();

private slots:
    void inputChanged(int index);

private:
    QGridLayout* m_layout;
    QComboBox* m_comboInput;
    QLabel* m_labelType;
    QLabel* m_label;

    IConditionWidget* m_baseWidget;
};

class ConditionInputButtonWidget : public IConditionWidget
{
    Q_OBJECT
public:
    ConditionInputButtonWidget(QWidget* parent);

    Condition* assemble();
    void edit(Condition *condition);

private:
    QComboBox* m_combo;
    QLabel* m_label;
};

class ConditionInputFaderWidget : public IConditionWidget
{
    Q_OBJECT
public:
    ConditionInputFaderWidget(QWidget* parent);

    Condition* assemble();
    void edit(Condition *condition);

private slots:
    void comboChanged(int index);
private:
    QComboBox* m_combo;
    QLabel* m_label;
    QSpinBox* m_spinBox;
};

// Variable Stuff

class ConditionVariableWidget : public IConditionWidget
{
    Q_OBJECT
public:
    ConditionVariableWidget(QWidget* parent, Script* script);
    void edit(Condition* condition);

    Condition* assemble();

private:
    QGridLayout* m_layout;
    QComboBox* m_combo;
    QLabel* m_label;
    QComboBox* m_comboTrigger;
    QLabel* m_labelTrigger;
    QSpinBox* m_spinValue;
};

#endif // ADDCONDITIONDIALOG_H
