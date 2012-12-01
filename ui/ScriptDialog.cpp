
#include "ui/ScriptDialog.h"
#include "ui_ScriptDialog.h"

#include "ui/RuleDialog.h"
#include "ui/VariableListDialog.h"

#include <QMessageBox>


ScriptDialog::ScriptDialog(QWidget *parent, Script *script, ConfigManager* config) :
    QDialog(parent),
    ui(new Ui::ScriptDialog),
    m_model(this, script)
{
    ui->setupUi(this);
    m_script = script;
    m_script->setConfig(config);
    m_config = config;

    ui->listRules->setModel(&m_model);

    ui->editName->setText(QString::fromStdString(script->getName()));
    ui->editDesc->setText(QString::fromStdString(script->getDescription()));

    // connect all signals - slots
    connect(ui->buttonAdd, SIGNAL(clicked()), this, SLOT(addRule()));
    connect(ui->buttonEdit, SIGNAL(clicked()), this, SLOT(editRule()));
    connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(deleteRule()));

    connect(ui->buttonEditVariables, SIGNAL(clicked()), this, SLOT(editVariables()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

ScriptDialog::~ScriptDialog()
{
    delete this->ui;
}

void ScriptDialog::okPressed()
{
    m_script->setName( ui->editName->text().toStdString() );
    m_script->setDescription( ui->editDesc->text().toStdString() );

    if(m_script->getName().empty())
    {
        // name is empty, we cannot save a script with an empty name
        QMessageBox(QMessageBox::Warning, "Empty Name", "The name of a script cannot be empty! Please fill in a name", QMessageBox::Ok, this).exec();
        return;
    }

    // TODO: check if a file with this name already exists, and if yes, abort

    this->done(Accepted);
}

void ScriptDialog::addRule()
{
    Rule* rule = new Rule();
    RuleDialog* dialog = new RuleDialog(this, rule, m_script);
    if( dialog->exec() == QDialog::Accepted )
    {
        m_script->addRule(rule);
        m_model.changed();
    }
    else
    {
        delete rule;
    }

    delete dialog;
}

void ScriptDialog::editRule()
{
    QModelIndexList indices = ui->listRules->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        RuleDialog* dialog = new RuleDialog(this, m_model.getRule(indices.front().row()), m_script);
        dialog->exec();
        m_model.changed();

        delete dialog;
    }
}

void ScriptDialog::deleteRule()
{
    QModelIndexList indices = ui->listRules->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        m_model.removeRow(indices.front().row());
    }
}

void ScriptDialog::editVariables()
{
    VariableListDialog* dialog = new VariableListDialog(this, m_script);

    dialog->exec();
}
