
#include "ui/OutputDCMotorFrame.h"
#include "util/Debug.h"

#include "ui_OutputDCMotorFrame.h"

OutputDCMotorFrame::OutputDCMotorFrame(HWOutputDCMotor* hw) : OutputFrame(hw), ui(new Ui::OutputDCMotorFrame)
{
    ui->setupUi(this);
    m_hw = hw;
    m_hw->registerOutputListener(this);

    ui->labelName->setText(QString(hw->getName().c_str()));
    ui->slider->setEnabled(m_hw->getOverride());

    connect(ui->buttonOverride, SIGNAL(clicked()), this, SLOT(overrideClicked()));
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged()));
    connect(ui->buttonFwd, SIGNAL(clicked()), this, SLOT(forwardClicked()));
    connect(ui->buttonRev, SIGNAL(clicked()), this, SLOT(reverseClicked()));
    connect(ui->buttonBrake, SIGNAL(clicked()), this, SLOT(brakeClicked()));
}

OutputDCMotorFrame::~OutputDCMotorFrame()
{
    m_hw->unregisterOutputListener(this);

    delete ui;
}

void OutputDCMotorFrame::overrideClicked()
{
    m_hw->setOverride(!m_hw->getOverride());

    if(m_hw->getOverride())
    {
        ui->buttonOverride->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 255, 0);"));
        ui->slider->setEnabled(true);
        ui->buttonFwd->setEnabled(true);
        ui->buttonRev->setEnabled(true);
        ui->buttonBrake->setEnabled(true);
    }
    else
    {
        ui->buttonOverride->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        ui->slider->setEnabled(false);
        ui->buttonFwd->setEnabled(false);
        ui->buttonRev->setEnabled(false);
        ui->buttonBrake->setEnabled(false);
    }
}

void OutputDCMotorFrame::forwardClicked()
{
    m_hw->setOverrideMotorState(HWOutputDCMotor::Forward);
}

void OutputDCMotorFrame::reverseClicked()
{
    m_hw->setOverrideMotorState(HWOutputDCMotor::Reverse);
}

void OutputDCMotorFrame::brakeClicked()
{
    m_hw->setOverrideMotorState(HWOutputDCMotor::Brake);
}

void OutputDCMotorFrame::sliderChanged()
{
    // only apply new value if override is enabled
    // If it is not, this is just a regular status update and should not update anything
    if(m_hw->getOverride())
    {
        m_hw->setOverrideSpeed(ui->slider->value());
    }
}

void OutputDCMotorFrame::onOutputChangedGUI()
{
    ui->labelState->setText( QString::fromStdString( HWOutputDCMotor::MotorStateToString( m_hw->getMotorState() ) ) );

    ui->progressBar->setValue( m_hw->getSpeed() );
    ui->slider->setValue( m_hw->getSpeed() );
}
