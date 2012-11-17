
#include "ui/OutputLEDFrame.h"
#include "util/Debug.h"

#include "ui_OutputLEDFrame.h"

OutputLEDFrame::OutputLEDFrame(HWOutputLED* hw) : OutputFrame(hw), ui(new Ui::OutputLEDFrame())
{
    ui->setupUi(this);
    m_hw = hw;
    m_hw->registerOutputListener(this);

    ui->labelName->setText(QString(hw->getName().c_str()));
    ui->slider->setEnabled(m_hw->getOverride());

    QObject::connect(ui->buttonOverride, SIGNAL(clicked()), this, SLOT(overrideClicked()));
    QObject::connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
}

OutputLEDFrame::~OutputLEDFrame()
{
    m_hw->unregisterOutputListener(this);

    delete ui;
}

void OutputLEDFrame::overrideClicked()
{
    m_hw->setOverride(!m_hw->getOverride());

    if(m_hw->getOverride())
    {
        ui->buttonOverride->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 255, 0);"));
        ui->slider->setEnabled(true);
    }
    else
    {
        ui->buttonOverride->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        ui->slider->setEnabled(false);
    }
}

void OutputLEDFrame::sliderMoved(int value)
{
    if(m_hw->getOverride())
    {
        m_hw->setOverrideValue((unsigned int)value);
    }
    else
    {
        pi_warn("Sliders has been moved, but override is not enabled! This should not be possible");
    }
}

void OutputLEDFrame::onOutputChangedGUI()
{
    ui->progressBar->setValue(m_hw->getValue());
    ui->slider->setValue(m_hw->getValue());
}
