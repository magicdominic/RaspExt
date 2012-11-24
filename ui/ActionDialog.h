#ifndef ACTIONDIALOG_H
#define ACTIONDIALOG_H

#include <script/Script.h>
#include <script/Action.h>

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>

namespace Ui {
    class ActionDialog;
}

class Script;
class IActionWidget;

class ActionDialog : public QDialog
{
    Q_OBJECT
public:
    ActionDialog(QWidget *parent, Script* script);
    ~ActionDialog();

    void edit(Action* action);

    Action* getAction() const { return m_action;}

private slots:
    void okPressed();
    void actionChanged(int index);

private:
    Ui::ActionDialog* ui;
    Action* m_action;
    Script* m_script;

    IActionWidget* m_baseWidget;
};


class IActionWidget : public QWidget
{
public:
    IActionWidget(QWidget* parent) : QWidget(parent) {};
    virtual Action* assemble() = 0;
    virtual void edit(Action* condition) = 0;
};

// Output Stuff
class ActionOutputWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputWidget(QWidget* parent, Script* script);
    void edit(Action* action);

    Action* assemble();

private slots:
    void outputChanged(int index);

private:
    QGridLayout* m_layout;
    QComboBox* m_comboOutput;
    QLabel* m_labelType;
    QLabel* m_label;

    Script* m_script;
    IActionWidget* m_baseWidget;
};

class ActionOutputRelayWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputRelayWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private:
    QComboBox* m_combo;
    QLabel* m_label;
};


class ActionOutputLEDWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputLEDWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private:
    QLabel* m_label;
    QSpinBox* m_spinBox;
};

class ActionOutputDCMotorWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputDCMotorWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private slots:
    void speedComboChanged(int index);

private:
    QLabel* m_labelState;
    QComboBox* m_comboState;
    QLabel* m_labelSpeed;
    QComboBox* m_comboSpeed;
    QSpinBox* m_spinSpeed;
};

class ActionOutputStepperWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputStepperWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private slots:
    void typeComboChanged(int index);

private:
    QLabel* m_label;
    QComboBox* m_combo;
    QGridLayout* m_layout;
    IActionWidget* m_baseWidget;
};

class ActionOutputStepperSoftStopWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputStepperSoftStopWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private:
    QLabel* m_label;
};

class ActionOutputStepperRunVelocityWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputStepperRunVelocityWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private:
    QLabel* m_label;
};

class ActionOutputStepperSetPositionWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputStepperSetPositionWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private:
    QLabel* m_label;
    QSpinBox* m_spinBox;
};

class ActionOutputStepperSetParamWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionOutputStepperSetParamWidget(QWidget* parent, Script* script);

    Action* assemble();
    void edit(Action* action);

private:
    QComboBox* m_comboIrun;
    QSpinBox* m_spinIrun;

    QComboBox* m_comboIhold;
    QSpinBox* m_spinIhold;

    QComboBox* m_comboVmax;
    QSpinBox* m_spinVmax;

    QComboBox* m_comboVmin;
    QSpinBox* m_spinVmin;

    QComboBox* m_comboMinSamples;
    QSpinBox* m_spinMinSamples;

    QComboBox* m_comboShaft;
    QSpinBox* m_spinShaft;

    QComboBox* m_comboAcc;
    QSpinBox* m_spinAcc;

    QComboBox* m_comboAbsThr;
    QSpinBox* m_spinAbsThr;

    QComboBox* m_comboDelThr;
    QSpinBox* m_spinDelThr;

    QComboBox* m_comboFS2StallEn;
    QSpinBox* m_spinFS2StallEn;

    QComboBox* m_comboAccShape;
    QSpinBox* m_spinAccShape;

    QComboBox* m_comboStepMode;
    QSpinBox* m_spinStepMode;

    QComboBox* m_comboDC100StEn;
    QSpinBox* m_spinDC100StEn;

    QComboBox* m_comboPWMJEn;
    QSpinBox* m_spinPWMJEn;

    QSpinBox* m_spinPWMfreq;

    QComboBox* m_comboSecPos;
    QSpinBox* m_spinSecPos;
};

// Variable Stuff

class ActionVariableWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionVariableWidget(QWidget* parent, Script* script);
    void edit(Action* action);
    Action* assemble();

private:
    QGridLayout* m_layout;
    QComboBox* m_combo;
    QLabel* m_label;
    QComboBox* m_comboOperator;
    QLabel* m_labelOperator;
    QSpinBox* m_spinOperand;
};

// Sleep Stuff
class ActionSleepWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionSleepWidget(QWidget* parent, Script* script);
    void edit(Action* action);
    Action* assemble();

private:
    QGridLayout* m_layout;
    QLabel* m_label;
    QLabel* m_labelMs;
    QSpinBox* m_spinWaitMs;
};

// CallRule Stuff
class ActionCallRuleWidget : public IActionWidget
{
    Q_OBJECT
public:
    ActionCallRuleWidget(QWidget* parent, Script* script);
    void edit(Action* action);
    Action* assemble();

private:
    QGridLayout* m_layout;
    QLabel* m_label;
    QComboBox* m_comboRule;
};

#endif // ACTIONDIALOG_H
