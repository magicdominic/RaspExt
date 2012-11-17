
#include "ui/OutputRelaisFrame.h"
#include "util/Debug.h"

#include "ui_OutputRelaisFrame.h"

OutputRelaisFrame::OutputRelaisFrame(HWOutputRelais* hw) : OutputFrame(hw), ui(new Ui::OutputRelaisFrame())
{
    ui->setupUi(this);
    m_hw = hw;
    m_hw->registerOutputListener(this);

    ui->labelName->setText(QString(hw->getName().c_str()));
    ui->buttonToggle->setEnabled(m_hw->getOverride());

    QObject::connect(ui->buttonOverride, SIGNAL(clicked()), this, SLOT(overrideClicked()));
    QObject::connect(ui->buttonToggle, SIGNAL(clicked()), this, SLOT(toggleClicked()));
}

OutputRelaisFrame::~OutputRelaisFrame()
{
    m_hw->unregisterOutputListener(this);

    delete ui;
}

void
OutputRelaisFrame::overrideClicked()
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
OutputRelaisFrame::toggleClicked()
{
    m_hw->setOverrideValue(!m_hw->getValue());
}

void OutputRelaisFrame::onOutputChangedGUI()
{
    if(m_hw->getValue())
    {
        ui->labelState->setText("ON");
        ui->labelState->setStyleSheet(QString::fromUtf8("background-color: rgb(144, 255, 41);"));
    }
    else
    {
        ui->labelState->setText("OFF");
        ui->labelState->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 93, 44);"));
    }
}
