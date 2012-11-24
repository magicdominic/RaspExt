
#include "ui/VariableEditDialog.h"
#include "ui_VariableEditDialog.h"

VariableEditDialog::VariableEditDialog(QWidget *parent, Variable* variable) :
    QDialog(parent),
    ui(new Ui::VariableEditDialog)
{
    ui->setupUi(this);

    m_variable = variable;

    // get values from variable
    ui->lineName->setText( QString::fromStdString( variable->getName() ) );
    ui->spinDefValue->setValue( variable->getDefaultValue() );

    // connect all signals-slots
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
}

VariableEditDialog::~VariableEditDialog()
{
    delete this->ui;
}

void VariableEditDialog::okPressed()
{
    m_variable->setDefaultValue( ui->spinDefValue->value() );
    m_variable->setName( ui->lineName->text().toStdString() );

    this->accept();
}
