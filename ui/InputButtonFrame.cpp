
#include "ui/InputButtonFrame.h"
#include "util/Debug.h"

#include "ui_InputButtonFrame.h"

InputButtonFrame::InputButtonFrame(HWInputButton* hw) : InputFrame(hw), ui(new Ui::InputButtonFrame())
{
    ui->setupUi(this);
    m_hw = hw;

    m_hw->registerInputListener(this);

    ui->labelName->setText(QString(m_hw->getName().c_str()));

    QObject::connect(ui->buttonOverride, SIGNAL(clicked()), this, SLOT(overrideClicked()));
    QObject::connect(ui->buttonToggle, SIGNAL(clicked()), this, SLOT(toggleClicked()));
}

InputButtonFrame::~InputButtonFrame()
{
    m_hw->unregisterInputListener(this);
    delete ui;
}

void
InputButtonFrame::overrideClicked()
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
InputButtonFrame::toggleClicked()
{
    m_hw->setOverrideValue(!m_hw->getValue());
}

void InputButtonFrame::onInputChangedGUI()
{
    InputFrame::onInputChangedGUI();

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
