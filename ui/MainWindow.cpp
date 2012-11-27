#include "ui/MainWindow.h"
#include "ui_MainWindow.h"
#include "ui/ScriptsTableModel.h"

#include "ui/InputFaderFrame.h"
#include "ui/InputButtonFrame.h"

#include "ui/OutputLEDFrame.h"
#include "ui/OutputRelayFrame.h"
#include "ui/OutputDCMotorFrame.h"
#include "ui/OutputStepperFrame.h"

#include "ui/ScriptDialog.h"

#include "util/Debug.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_config(this)
{
    ui->setupUi(this);

    ui->layoutInput->setAlignment(Qt::AlignTop);
    ui->layoutOutput->setAlignment(Qt::AlignTop);
    ui->layoutVariable->setAlignment(Qt::AlignTop);


    m_scriptsModel = new ScriptsTableModel(ui->tableScripts);

    ui->tableScripts->setModel(m_scriptsModel);
    ui->tableScripts->horizontalHeader()->setStretchLastSection(true);
    ui->tableScripts->setColumnWidth(0, 200);


    // connect all signals-slots
    connect(ui->buttonCreateScript, SIGNAL(clicked()), this, SLOT(createScript()));
    connect(ui->buttonSelectScript, SIGNAL(clicked()), this, SLOT(selectScript()));
    connect(ui->buttonEditScript, SIGNAL(clicked()), this, SLOT(editScript()));
    connect(ui->buttonDeleteScript, SIGNAL(clicked()), this, SLOT(deleteScript()));

    connect(ui->buttonStop, SIGNAL(clicked()), this, SLOT(stopScript()));
    connect(ui->buttonPlayPause, SIGNAL(clicked()), this, SLOT(startPauseScript()));

    // load standard config
    m_config.load("config_test01.xml");
    m_config.init();
}

MainWindow::~MainWindow()
{
    m_config.save("config_test01.xml");
    m_config.deinit();
    m_config.clear(); // has to be cleared before ui is deleted, otherwise ui widgets are no longer available and this would lead to a segfault

    delete m_scriptsModel;
    delete ui;
}


void MainWindow::createScript()
{
    Script* script = new Script();
    ScriptDialog* dialog = new ScriptDialog(this, script, &m_config);

    if( dialog->exec() == QDialog::Accepted )
    {
        // save script to hd
        script->save();

        // add it to the gui
        m_scriptsModel->addScript(script);
    }

    delete dialog;
}


void MainWindow::editScript()
{
    QModelIndexList indices = ui->tableScripts->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        Script* script = m_scriptsModel->getScript(indices.front().row());

        // check if script is selected, if yes, then display a warning and don't do anything
        if(script == m_config.getActiveScript())
        {
            QMessageBox(QMessageBox::Warning, "Warning", "Cannot edit script because it is selected.\nPlease stop the script first", QMessageBox::Ok, this).exec();
            return;
        }

        // check if all required stuff exists
        // if not and the user decides that this is not acceptable, we stop immediatly
        if( !this->checkScript(script) )
            return;

        // now we can create the dialog an start editing this script
        ScriptDialog* dialog = new ScriptDialog(this, script, &m_config);

        if( dialog->exec() == QDialog::Accepted)
        {
            // save script to filesystem
            script->save();

            // this updates the description (and name) of a script in the ui
            m_scriptsModel->changed();
        }

        delete dialog;
    }
}

void MainWindow::deleteScript()
{
    QModelIndexList indices = ui->tableScripts->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        // check if script is selected, if yes, then display a warning and don't do anything
        if(m_config.getActiveScript() == m_scriptsModel->getScript(indices.front().row()))
        {
            QMessageBox(QMessageBox::Warning, "Warning", "Cannot delete script because it is selected.\nPlease stop the script first", QMessageBox::Ok, this).exec();
            return;
        }

        m_scriptsModel->removeRow(indices.front().row());
    }
}

void MainWindow::selectScript()
{
    QModelIndexList indices = ui->tableScripts->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        Script* script = m_scriptsModel->getScript(indices.front().row());

        // check if all required stuff exists
        // if not and the user decides that this is not acceptable, we stop immediatly
        if( !this->checkScript(script) )
            return;


        m_config.setActiveScript( script );

        this->updateScriptState();
    }
}

void MainWindow::stopScript()
{
    m_config.stopActiveScript();

    this->updateScriptState();
}

void MainWindow::startPauseScript()
{
    if(m_config.getActiveScriptState() == ConfigManager::Active)
    {
        m_config.pauseActiveScript();

        this->updateScriptState();
    }
    else if(m_config.getActiveScriptState() == ConfigManager::Paused)
    {
        m_config.continueActiveScript();

        this->updateScriptState();
    }
}

void MainWindow::updateScriptState()
{
    switch( m_config.getActiveScriptState() )
    {
    case ConfigManager::Inactive:
        ui->labelState->setText( "Inactive" );
        break;
    case ConfigManager::Active:
        ui->labelState->setText( "Active" );
        break;
    case ConfigManager::Paused:
        ui->labelState->setText( "Paused" );
        break;
    }

    Script* script = m_config.getActiveScript();
    if(script != NULL)
        ui->labelScript->setText( QString::fromStdString( script->getName() ) );
    else
        ui->labelScript->setText("");
}

/**
 * @brief MainWindow::checkScript This function checks all prerequisits of a script (e.g. inputs, outputs, variables).
 * If it finds unmet prerequisits, it asks the user what to do.
 * If everything looks good or the user accepts the unmet dependencies, we return true, false otherwise
 * @param script
 * @return
 */
bool MainWindow::checkScript(Script *script)
{
    std::list<Rule::RequiredInput> listInput;
    std::list<Rule::RequiredOutput> listOutput;
    std::list<Rule::RequiredVariable> listVariable;

    script->getRequiredList(&listInput, &listOutput, &listVariable);

    // Now we have the list of requirements, we now go through each list and delete all items which are present
    // In the end we have lists containing only the unmet dependencies
    for(std::list<Rule::RequiredInput>::iterator it = listInput.begin(); it != listInput.end(); it++)
    {
        HWInput* input = m_config.getInputByName((*it).name);
        if(input != NULL && input->getType() == (*it).type)
        {
            listInput.erase(it--);
        }
    }

    for(std::list<Rule::RequiredOutput>::iterator it = listOutput.begin(); it != listOutput.end(); it++)
    {
        HWOutput* output = m_config.getOutputByName((*it).name);
        if(output != NULL && output->getType() == (*it).type)
        {
            listOutput.erase(it--);
        }
    }

    // now the tricky part, variables
    std::list<Variable*> listVariableScript = script->getVariableList();
    for(std::list<Rule::RequiredVariable>::iterator it = listVariable.begin(); it != listVariable.end(); it++)
    {
        for(std::list<Variable*>::iterator varIt = listVariableScript.begin(); varIt != listVariableScript.end(); varIt++)
        {
            if( (*it).name.compare( (*varIt)->getName() ) == 0 )
            {
                listVariable.erase(it--);
                break;
            }
        }
    }


    if( !listInput.empty() || !listOutput.empty() || !listVariable.empty())
    {
        QString message = "Not all required inputs, outputs or variables are present. The following parts are missing:\n\n";

        for(std::list<Rule::RequiredInput>::iterator it = listInput.begin(); it != listInput.end(); it++)
        {
            message.append( "Input ");
            message.append( QString::fromStdString((*it).name) );
            message.append( " with type ");
            message.append( QString::fromStdString( HWInput::HWInputTypeToString((*it).type) ) );
            message.append( "\n" );
        }

        for(std::list<Rule::RequiredOutput>::iterator it = listOutput.begin(); it != listOutput.end(); it++)
        {
            message.append( "Output ");
            message.append( QString::fromStdString((*it).name) );
            message.append( " with type ");
            message.append( QString::fromStdString( HWOutput::HWOutputTypeToString((*it).type) ) );
            message.append( "\n" );
        }

        for(std::list<Rule::RequiredVariable>::iterator it = listVariable.begin(); it != listVariable.end(); it++)
        {
            message.append( "Variable " );
            message.append( QString::fromStdString((*it).name) );
            message.append( "\n" );
        }

        message.append("\nDo you want to continue without these parts?");

        QMessageBox messageBox(QMessageBox::Warning, "Error",
                               message,
                               QMessageBox::Yes|QMessageBox::No, this);

        if( messageBox.exec() == QMessageBox::Yes )
            return true;
        else
            return false;
    }

    return true;
}

void MainWindow::addInput(HWInput* hw)
{
    InputFrame* frame = NULL;
    switch(hw->getType())
    {
    case HWInput::Button:
        frame = new InputButtonFrame((HWInputButton*)hw);

        break;

    case HWInput::Fader:
        frame = new InputFaderFrame((HWInputFader*)hw);
        break;

    default:
        pi_warn("Requested GUI-object for unknown input type");
        break;
    }

    if(frame != NULL)
    {
        ui->layoutInput->addWidget(frame);
        m_listInputFrame.push_back(frame);
    }
}

void MainWindow::addOutput(HWOutput* hw)
{
    OutputFrame* frame = NULL;

    switch(hw->getType())
    {
    case HWOutput::Relay:
        frame = new OutputRelayFrame((HWOutputRelay*)hw);
        break;
    case HWOutput::DCMotor:
        frame = new OutputDCMotorFrame((HWOutputDCMotor*)hw);
        break;
    case HWOutput::LED:
        frame = new OutputLEDFrame((HWOutputLED*)hw);
        break;
    case HWOutput::Stepper:
        frame = new OutputStepperFrame((HWOutputStepper*)hw);
        break;
    default:
        pi_warn("Requested GUI-object for unknown output type");
        break;
    }

    if(frame != NULL)
    {
        ui->layoutOutput->addWidget(frame);
        m_listOutputFrame.push_back(frame);
    }
}

void MainWindow::addVariable(Variable* var)
{
    VariableFrame* frame = new VariableFrame(var);
    ui->layoutVariable->addWidget(frame);
    m_listVarFrame.push_back( frame );
}

void MainWindow::removeInput(HWInput* hw)
{
    for(std::list<InputFrame*>::iterator it = m_listInputFrame.begin(); it != m_listInputFrame.end(); it++)
    {
        if((*it)->getHW() == hw)
        {
            ui->layoutInput->removeWidget(*it);
            delete (*it);
            break;
        }
    }
}

void MainWindow::removeOutput(HWOutput* hw)
{
    for(std::list<OutputFrame*>::iterator it = m_listOutputFrame.begin(); it != m_listOutputFrame.end(); it++)
    {
        if((*it)->getHW() == hw)
        {
            ui->layoutOutput->removeWidget(*it);
            delete (*it);
            break;
        }
    }
}

void MainWindow::removeVariable(Variable* var)
{
    for(std::list<VariableFrame*>::iterator it = m_listVarFrame.begin(); it != m_listVarFrame.end(); it++)
    {
        if((*it)->getVariable() == var)
        {
            ui->layoutVariable->removeWidget(*it);
            delete (*it);
            break;
        }
    }
}
