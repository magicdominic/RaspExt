
#include "ui/ActionDialog.h"
#include "ui_ActionDialog.h"

#include "script/ActionOutput.h"
#include "script/ActionOutputLED.h"
#include "script/ActionOutputDCMotor.h"
#include "script/ActionOutputRelais.h"
#include "script/ActionOutputStepper.h"
#include "script/ActionOutputStepperSoftStop.h"
#include "script/ActionOutputStepperRunVelocity.h"
#include "script/ActionVariable.h"
#include "script/ActionSleep.h"
#include "script/ActionCallRule.h"
#include "hw/HWInput.h"
#include "util/Debug.h"

ActionDialog::ActionDialog(QWidget *parent, Script *script) :
    QDialog(parent),
    ui(new Ui::ActionDialog)
{
    pi_assert(script != NULL);

    ui->setupUi(this);

    m_script = script;
    m_action = NULL;
    m_baseWidget = NULL;

    // connect all signals - slots
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->comboAction, SIGNAL(currentIndexChanged(int)), this, SLOT(actionChanged(int)));

    // default values
    this->actionChanged( ui->comboAction->currentIndex() );
}

void ActionDialog::actionChanged(int index)
{
    if(m_baseWidget != NULL)
    {
        ui->frameGrid->removeWidget(m_baseWidget);
        delete m_baseWidget;
        m_baseWidget = NULL;
    }

    Action::Type type = (Action::Type)index;

    switch(type)
    {
    case Action::Output:
        m_baseWidget = new ActionOutputWidget(this, m_script);
        break;

    case Action::Var:
        m_baseWidget = new ActionVariableWidget(this, m_script);
        break;

    case Action::Sleep:
        m_baseWidget = new ActionSleepWidget(this, m_script);
        break;

    case Action::CallRule:
        m_baseWidget = new ActionCallRuleWidget(this, m_script);
        break;
    }

    if(m_baseWidget != NULL)
    {
        ui->frameGrid->addWidget(m_baseWidget, 1, 0, 1, 2);
    }
}

void ActionDialog::okPressed()
{
    pi_assert(m_action == NULL);
    pi_assert(m_baseWidget != NULL);

    m_action = ((IActionWidget*)m_baseWidget)->assemble();

    if(m_action == NULL)
        pi_warn("Assembling Action failed");

    this->done(Accepted);
}

void ActionDialog::edit(Action *action)
{
    ui->comboAction->setCurrentIndex(action->getType());
    if(m_baseWidget != NULL)
    {
        m_baseWidget->edit(action);
    }
}

ActionDialog::~ActionDialog()
{
    delete ui;
}



ActionOutputWidget::ActionOutputWidget(QWidget* parent, Script *script) : IActionWidget(parent)
{
    m_baseWidget = NULL;
    m_script = script;

    m_comboOutput = new QComboBox(this);
    m_label = new QLabel("Select input", this);
    m_labelType = new QLabel("");

    // fill combo box with all outputs
    std::list<HWOutput*> listOutputs = script->getOutputList();
    for(std::list<HWOutput*>::iterator it = listOutputs.begin(); it != listOutputs.end(); it++)
    {
        m_comboOutput->addItem((*it)->getName().c_str(), qVariantFromValue((void*)*it));
    }

    m_layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    m_layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    m_layout->addWidget(m_labelType, 0, 0);
    m_layout->addWidget(m_comboOutput, 0, 1);
    m_layout->addWidget(m_labelType, 1, 0, 1, 2);

    this->setLayout(m_layout);

    // connect all signals - slots
    connect(m_comboOutput, SIGNAL(currentIndexChanged(int)), this, SLOT(outputChanged(int)));

    // default values
    this->outputChanged( m_comboOutput->currentIndex() );
}

void ActionOutputWidget::outputChanged(int index)
{
    QVariant variant = m_comboOutput->itemData( index );
    HWOutput* hw = (HWOutput*)variant.value<void*>();

    HWOutput::HWOutputType type = hw->getType();

    QString outputtype = "Selected output type is ";
    outputtype.append( HWOutput::HWOutputTypeToString( type ).c_str() );

    m_labelType->setText( outputtype );

    // delete old baseWidget if it exists
    if(m_baseWidget != NULL)
    {
        m_layout->removeWidget(m_baseWidget);
        delete m_baseWidget;
        m_baseWidget = NULL;
    }

    switch(type)
    {
    case HWOutput::Relais:
        m_baseWidget = new ActionOutputRelaisWidget(this, m_script);
        break;
    case HWOutput::DCMotor:
        m_baseWidget = new ActionOutputDCMotorWidget(this, m_script);
        break;
    case HWOutput::LED:
        m_baseWidget = new ActionOutputLEDWidget(this, m_script);
        break;
    case HWOutput::Stepper:
        m_baseWidget = new ActionOutputStepperWidget(this, m_script);
        break;

    default:
        pi_warn("Unknown output type");
        break;
    }

    // add new baseWidget, this should never be NULL. If it is, somewhere something went wrong
    if(m_baseWidget != NULL)
    {
        m_layout->addWidget(m_baseWidget, 2, 0, 1, 2);
    }
}

void ActionOutputWidget::edit(Action* act)
{
    ActionOutput* action = (ActionOutput*)act;

    QString str = QString::fromStdString( action->getHWName() );

    for(unsigned int i = 0; i < m_comboOutput->count(); i++)
    {
        if( m_comboOutput->itemText(i).compare(str, Qt::CaseInsensitive) == 0 )
        {
            m_comboOutput->setCurrentIndex(i);
            break;
        }
    }

    if( m_baseWidget != NULL)
    {
        m_baseWidget->edit(action);
    }
}

Action* ActionOutputWidget::assemble()
{
    pi_assert(m_baseWidget != NULL);

    ActionOutput* action = (ActionOutput*) ((IActionWidget*)m_baseWidget)->assemble();

    action->setHWName( m_comboOutput->currentText().toStdString() );

    return action;
}


ActionOutputRelaisWidget::ActionOutputRelaisWidget(QWidget* parent, Script *script) : IActionWidget(parent)
{
    // create comboBox and add items
    m_combo = new QComboBox(this);
    m_combo->addItem("Off");
    m_combo->addItem("On");
    m_combo->addItem("Toggle");

    m_label = new QLabel("Set relais to", this);

    QGridLayout* layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    layout->addWidget(m_label, 0, 0);
    layout->addWidget(m_combo, 0, 1);

    this->setLayout(layout);
}

Action* ActionOutputRelaisWidget::assemble()
{
    ActionOutputRelais* action = new ActionOutputRelais();

    action->setState( (ActionOutputRelais::State)m_combo->currentIndex() );

    return action;
}

void ActionOutputRelaisWidget::edit(Action* act)
{
    ActionOutputRelais* action = (ActionOutputRelais*)act;
    m_combo->setCurrentIndex( action->getState());
}


ActionOutputLEDWidget::ActionOutputLEDWidget(QWidget* parent, Script *script) : IActionWidget(parent)
{
    m_label = new QLabel("Set brightness of LED to ", this);
    m_spinBox = new QSpinBox(this);
    m_spinBox->setMinimum(0);
    m_spinBox->setMaximum(100);

    QGridLayout* layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    layout->addWidget(m_label, 0, 0);
    layout->addWidget(m_spinBox, 0, 1);

    this->setLayout(layout);
}

Action* ActionOutputLEDWidget::assemble()
{
    ActionOutputLED* action = new ActionOutputLED();

    action->setValue( m_spinBox->value() );

    return action;
}

void ActionOutputLEDWidget::edit(Action* act)
{
    ActionOutputLED* action = (ActionOutputLED*)act;

    m_spinBox->setValue( action->getValue() );
}

ActionOutputDCMotorWidget::ActionOutputDCMotorWidget(QWidget* parent, Script* script) : IActionWidget(parent)
{
    m_labelState = new QLabel("Set motor to", this);
    // create comboBox and add items
    m_comboState = new QComboBox(this);
    m_comboState->addItem("Standby");
    m_comboState->addItem("Reverse");
    m_comboState->addItem("Forward");
    m_comboState->addItem("Brake");

    m_labelSpeed = new QLabel("Set speed to", this);

    // create comboBox to select a fader
    m_comboSpeed = new QComboBox(this);
    m_comboSpeed->addItem("value");
    std::list<HWInput*> listInputs = script->getInputList();
    for(std::list<HWInput*>::iterator it = listInputs.begin(); it != listInputs.end(); it++)
    {
        if( (*it)->getType() == HWInput::Fader )
            m_comboSpeed->addItem((*it)->getName().c_str());
    }

    m_spinSpeed = new QSpinBox(this);
    m_spinSpeed->setMaximum(100);
    m_spinSpeed->setMinimum(0);

    QGridLayout* layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    layout->addWidget(m_labelState, 0, 0);
    layout->addWidget(m_comboState, 0, 1, 1, 2);
    layout->addWidget(m_labelSpeed, 1, 0);
    layout->addWidget(m_comboSpeed, 1, 1);
    layout->addWidget(m_spinSpeed, 1, 2);

    this->setLayout(layout);

    // connect all signals-slots
    connect(m_comboSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(speedComboChanged(int)));
}

void ActionOutputDCMotorWidget::speedComboChanged(int index)
{
    if( index != 0 )
        m_spinSpeed->hide();
    else
        m_spinSpeed->show();
}

Action* ActionOutputDCMotorWidget::assemble()
{
    ActionOutputDCMotor* action = new ActionOutputDCMotor();

    action->setState( (HWOutputDCMotor::MotorState)m_comboState->currentIndex() );
    action->setSpeed( m_spinSpeed->value() );

    if( m_comboSpeed->currentIndex() != 0 )
    {
        action->setInputName( m_comboSpeed->currentText().toStdString() );
    }

    return action;
}

void ActionOutputDCMotorWidget::edit(Action* act)
{
    ActionOutputDCMotor* action = (ActionOutputDCMotor*)act;

    m_comboState->setCurrentIndex( action->getState() );
    m_spinSpeed->setValue( action->getSpeed() );


    // only set this if it is used
    if( !action->getInputName().empty() )
    {
        QString str = QString::fromStdString( action->getInputName() );

        for(unsigned int i = 0; i < m_comboSpeed->count(); i++)
        {
            if( m_comboSpeed->itemText(i).compare(str, Qt::CaseInsensitive) == 0 )
            {
                m_comboSpeed->setCurrentIndex(i);
                break;
            }
        }
    }
}

ActionOutputStepperWidget::ActionOutputStepperWidget(QWidget* parent, Script* script) : IActionWidget(parent)
{
    m_baseWidget = NULL;

    m_label = new QLabel("Select type", this);

    // create comboBox for all possible types
    m_combo = new QComboBox(this);
    m_combo->addItem("Soft stop");
    m_combo->addItem("RunVelocity");

    m_layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    m_layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    m_layout->addWidget(m_label, 0, 0);
    m_layout->addWidget(m_combo, 0, 1);

    this->setLayout(m_layout);

    // connect all signals-slots
    connect(m_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(typeComboChanged(int)));

    // default values
    this->typeComboChanged( m_combo->currentIndex() );
}

void ActionOutputStepperWidget::typeComboChanged(int index)
{
    if(m_baseWidget != NULL)
    {
        m_layout->removeWidget(m_baseWidget);
        delete m_baseWidget;
        m_baseWidget = NULL;
    }

    switch(index)
    {
    case ActionOutputStepper::SoftStop:
        m_baseWidget = new ActionOutputStepperSoftStopWidget(this, NULL); // we do not need Script in this class, so we just take NULL
        break;
    case ActionOutputStepper::RunVelocity:
        m_baseWidget = new ActionOutputStepperRunVelocityWidget(this, NULL); // we do not need Script in this class, so we just take NULL
        break;
    }

    // add new baseWidget, this should never be NULL. If it is, somewhere something went wrong
    if(m_baseWidget != NULL)
    {
        m_layout->addWidget(m_baseWidget, 2, 0, 1, 2);
    }
}

Action* ActionOutputStepperWidget::assemble()
{
    if( m_baseWidget != NULL)
        return m_baseWidget->assemble();

    return NULL;
}

void ActionOutputStepperWidget::edit(Action* act)
{
    ActionOutputStepper* action = (ActionOutputStepper*)act;

    m_combo->setCurrentIndex( action->getStepperType() );

    if( m_baseWidget != NULL)
    {
        m_baseWidget->edit(action);
    }
}

ActionOutputStepperSoftStopWidget::ActionOutputStepperSoftStopWidget(QWidget* parent, Script* script) : IActionWidget(parent)
{
    m_label = new QLabel("Do a soft stop", this);

    QGridLayout* layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    layout->addWidget(m_label, 0, 0);

    this->setLayout(layout);
}

Action* ActionOutputStepperSoftStopWidget::assemble()
{
    return new ActionOutputStepperSoftStop();
}

void ActionOutputStepperSoftStopWidget::edit(Action* act)
{
    // nothing to do
}

ActionOutputStepperRunVelocityWidget::ActionOutputStepperRunVelocityWidget(QWidget* parent, Script* script) : IActionWidget(parent)
{
    m_label = new QLabel("Send command RunVelocity to the stepper", this);

    QGridLayout* layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    layout->addWidget(m_label, 0, 0);

    this->setLayout(layout);
}

Action* ActionOutputStepperRunVelocityWidget::assemble()
{
    return new ActionOutputStepperRunVelocity();
}

void ActionOutputStepperRunVelocityWidget::edit(Action* act)
{
    // nothing to do
}


ActionVariableWidget::ActionVariableWidget(QWidget* parent, Script *script) : IActionWidget(parent)
{
    m_combo = new QComboBox(this);
    m_label = new QLabel("Select variable", this);

    // fill combo box with all variables
    std::list<Variable*> listVariable = script->getVariableList();
    for(std::list<Variable*>::iterator it = listVariable.begin(); it != listVariable.end(); it++)
    {
        m_combo->addItem((*it)->getName().c_str());
    }

    m_comboOperator = new QComboBox(this);
    m_comboOperator->addItem("=");
    m_comboOperator->addItem("+=");
    m_comboOperator->addItem("-=");

    m_labelOperator = new QLabel("Set variable", this);

    m_spinOperand = new QSpinBox(this);
    m_spinOperand->setMinimum(-100);
    m_spinOperand->setMaximum(100);


    m_layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    m_layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    m_layout->addWidget(m_label, 0, 0);
    m_layout->addWidget(m_combo, 0, 1, 1, 2);
    m_layout->addWidget(m_labelOperator, 1, 0);
    m_layout->addWidget(m_comboOperator, 1, 1);
    m_layout->addWidget(m_spinOperand, 1, 2);

    this->setLayout(m_layout);
}

void ActionVariableWidget::edit(Action* act)
{
    ActionVariable* action = (ActionVariable*)act;

    QString str = QString::fromStdString( action->getVarName() );

    for(unsigned int i = 0; i < m_combo->count(); i++)
    {
        if( m_combo->itemText(i).compare(str, Qt::CaseInsensitive) == 0 )
        {
            m_combo->setCurrentIndex(i);
            break;
        }
    }

    m_comboOperator->setCurrentIndex( action->getOperator() );
    m_spinOperand->setValue( action->getOperand() );
}

Action* ActionVariableWidget::assemble()
{
    ActionVariable* action = new ActionVariable();

    action->setVarName( m_combo->currentText().toStdString() );
    action->setOperator( (ActionVariable::Operator)m_comboOperator->currentIndex() );
    action->setOperand( m_spinOperand->value() );

    return action;
}

ActionSleepWidget::ActionSleepWidget(QWidget* parent, Script *script) : IActionWidget(parent)
{
    m_label = new QLabel("Sleep for", this);

    m_spinWaitMs = new QSpinBox(this);
    m_spinWaitMs->setMinimum(0);
    m_spinWaitMs->setMaximum(30 * 1000);

    m_labelMs = new QLabel("ms", this);


    m_layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    m_layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    m_layout->addWidget(m_label, 0, 0);
    m_layout->addWidget(m_spinWaitMs, 0, 1);
    m_layout->addWidget(m_labelMs, 0, 2);

    this->setLayout(m_layout);
}

void ActionSleepWidget::edit(Action* act)
{
    ActionSleep* action = (ActionSleep*)act;

    m_spinWaitMs->setValue( action->getWaitMs() );
}

Action* ActionSleepWidget::assemble()
{
    ActionSleep* action = new ActionSleep();

    action->setWaitMs( m_spinWaitMs->value() );

    return action;
}

ActionCallRuleWidget::ActionCallRuleWidget(QWidget* parent, Script *script) : IActionWidget(parent)
{
    m_label = new QLabel("Select rule", this);

    m_comboRule = new QComboBox(this);

    // fill combo box with all rule names
    std::vector<Rule*> list = script->getRuleList();
    for(std::vector<Rule*>::iterator it = list.begin(); it != list.end(); it++)
    {
        m_comboRule->addItem((*it)->getName().c_str());
    }


    m_layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    m_layout->setContentsMargins(0, 0, 0, 0);

    // add our widgets to the layout
    m_layout->addWidget(m_label, 0, 0);
    m_layout->addWidget(m_comboRule, 0, 1);

    this->setLayout(m_layout);
}

void ActionCallRuleWidget::edit(Action* act)
{
    ActionCallRule* action = (ActionCallRule*)act;

    QString str = QString::fromStdString( action->getRuleName() );

    for(unsigned int i = 0; i < m_comboRule->count(); i++)
    {
        if( m_comboRule->itemText(i).compare(str, Qt::CaseInsensitive) == 0 )
        {
            m_comboRule->setCurrentIndex(i);
            break;
        }
    }
}

Action* ActionCallRuleWidget::assemble()
{
    ActionCallRule* action = new ActionCallRule();

    action->setRuleName( m_comboRule->currentText().toStdString() );

    return action;
}
