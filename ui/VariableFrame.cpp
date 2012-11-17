
#include "ui/VariableFrame.h"
#include "ui_VariableFrame.h"
#include "util/Debug.h"

VariableFrame::VariableFrame(Variable *variable) : ui(new Ui::VariableFrame)
{
    ui->setupUi(this);

    m_variable = variable;
    m_variable->registerVariableListener(this);

    ui->label->setText( QString::fromStdString( variable->getName() ) );
    ui->spinBox->setValue( variable->getValue() );

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    QObject::connect(this, SIGNAL(onVariableChangedSignal()), this, SLOT(onVariableChangedGUI()), Qt::QueuedConnection);
}

VariableFrame::~VariableFrame()
{
    m_variable->unregisterVariableListener(this);
    delete ui;
}

void VariableFrame::onVariableChanged(Variable* var)
{
    pi_assert(var == m_variable);
    emit onVariableChangedSignal();
}

void VariableFrame::onVariableChangedGUI()
{
    ui->spinBox->setValue( m_variable->getValue() );
}

void VariableFrame::valueChanged(int value)
{
    m_variable->setValue(value);
}
