
#include "ui/VariableListDialog.h"
#include "script/Script.h"
#include "ui/VariableEditDialog.h"
#include "ui_VariableListDialog.h"
#include "util/Debug.h"

VariableListDialog::VariableListDialog(QWidget *parent, Script *script) :
    QDialog(parent),
    ui(new Ui::VariableListDialog)
{
    ui->setupUi(this);
    m_script = script;

    ui->listVariables->setModel(&m_model);
    ui->listVariables->horizontalHeader()->setStretchLastSection(true);

    // fill list
    std::list<Variable*> list = m_script->getVariableList();
    for(std::list<Variable*>::iterator it = list.begin(); it != list.end(); it++)
    {
        // clone the variables to be able to discard all changes we mode
        m_model.addVariable((*it)->clone());
    }

    // connect all signals - slots
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->buttonAdd, SIGNAL(clicked()), this, SLOT(addVariable()));
    connect(ui->buttonEdit, SIGNAL(clicked()), this, SLOT(editVariable()));
    connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(deleteVariable()));
}

VariableListDialog::~VariableListDialog()
{
    delete this->ui;
}

void VariableListDialog::okPressed()
{
    // update variables in script
    m_script->clearVariables();
    for(int i = 0; i < m_model.rowCount(); i++)
    {
        // clone the variables as the ones in the list will be deleted by the list
        m_script->addVariable( m_model.getVariable(i)->clone() );
    }

    this->accept();
}

void VariableListDialog::addVariable()
{
    Variable* variable = new Variable();

    VariableEditDialog* dialog = new VariableEditDialog(this, variable);

    // Check if the user clicked ok
    // If yes, then add the variable to our script, otherwise discard it
    if( dialog->exec() == Accepted)
    {
        m_model.addVariable(variable);
    }
    else
    {
        delete variable;
    }

    delete dialog;
}

void VariableListDialog::editVariable()
{
    QModelIndexList indices = ui->listVariables->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        Variable* variable = m_model.getVariable(indices.front().row())->clone();

        VariableEditDialog* dialog = new VariableEditDialog(this, variable);

        // Check if the user clicked ok
        // If yes, then add the variable to our script, otherwise discard it
        if( dialog->exec() == Accepted)
        {
            m_model.removeRow(indices.front().row());
            m_model.addVariable(variable);
        }
        else
        {
            delete variable;
        }

        delete dialog;
    }
}

void VariableListDialog::deleteVariable()
{
    QModelIndexList indices = ui->listVariables->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        m_model.removeRow(indices.front().row());
    }
}
