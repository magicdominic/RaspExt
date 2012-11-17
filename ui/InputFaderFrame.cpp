#include "ui/InputFaderFrame.h"
#include "util/Debug.h"

#include "ui_InputFaderFrame.h"

InputFaderFrame::InputFaderFrame(HWInputFader* hw) : InputFrame(hw) , ui(new Ui::InputFaderFrame())
{
    ui->setupUi(this);
    m_hw = hw;

    m_hw->registerInputListener(this);

    ui->labelName->setText(QString(hw->getName().c_str()));
    ui->slider->setEnabled(m_hw->getOverride());

    QObject::connect(ui->buttonOverride, SIGNAL(clicked()), this, SLOT(overrideClicked()));
    QObject::connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
}

InputFaderFrame::~InputFaderFrame()
{
    m_hw->unregisterInputListener(this);
    delete ui;
}

void
InputFaderFrame::overrideClicked()
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

void InputFaderFrame::sliderMoved(int value)
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

void InputFaderFrame::onInputChangedGUI()
{
    InputFrame::onInputChangedGUI();

    ui->progressBar->setValue(m_hw->getValue());
    ui->slider->setValue(m_hw->getValue());
}
