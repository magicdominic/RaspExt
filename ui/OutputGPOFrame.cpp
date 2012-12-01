
#include "ui/OutputGPOFrame.h"
#include "util/Debug.h"

#include "ui_OutputGPOFrame.h"

OutputGPOFrame::OutputGPOFrame(HWOutputGPO* hw) : OutputFrame(hw), ui(new Ui::OutputGPOFrame)
{
    ui->setupUi(this);
    m_hw = hw;
    m_hw->registerOutputListener(this);

    ui->labelName->setText(QString(hw->getName().c_str()));
    ui->buttonToggle->setEnabled(m_hw->getOverride());

    connect(ui->buttonOverride, SIGNAL(clicked()), this, SLOT(overrideClicked()));
    connect(ui->buttonToggle, SIGNAL(clicked()), this, SLOT(toggleClicked()));
}

OutputGPOFrame::~OutputGPOFrame()
{
    m_hw->unregisterOutputListener(this);

    delete ui;
}

void
OutputGPOFrame::overrideClicked()
{
    m_hw->setOverride(!m_hw->getOverride());

    if(m_hw->getOverride())
    {
        ui->buttonOverride->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 255, 0);"));
        ui->buttonToggle->setEnabled(true);
    }
    else
    {
        ui->buttonOverride->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        ui->buttonToggle->setEnabled(false);
    }
}

void
OutputGPOFrame::toggleClicked()
{
    m_hw->setOverrideValue(!m_hw->getValue());
}

void OutputGPOFrame::onOutputChangedGUI()
{
    if(m_hw->getValue())
    {
        ui->labelState->setText("HIGH");
        ui->labelState->setStyleSheet(QString::fromUtf8("background-color: rgb(144, 255, 41);"));
    }
    else
    {
        ui->labelState->setText("LOW");
        ui->labelState->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 93, 44);"));
    }
}
