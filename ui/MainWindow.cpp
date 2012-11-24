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
        m_config.setActiveScript( m_scriptsModel->getScript(indices.front().row()) );

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
