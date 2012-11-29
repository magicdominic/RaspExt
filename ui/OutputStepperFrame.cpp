
#include "ui/OutputStepperFrame.h"
#include "ui/OutputStepperDetailsDialog.h"
#include "util/Debug.h"

#include "ui_OutputStepperFrame.h"

OutputStepperFrame::OutputStepperFrame(HWOutputStepper* hw) : OutputFrame(hw), ui(new Ui::OutputStepperFrame())
{
    ui->setupUi(this);
    m_hw = hw;
    m_hw->registerOutputListener(this);

    ui->labelName->setText(QString(hw->getName().c_str()));

    // connect all signals - slots
    connect(ui->buttonDetails, SIGNAL(clicked()), this, SLOT(detailsClicked()));
    connect(ui->buttonRunVelocity, SIGNAL(clicked()), this, SLOT(runVelocityClicked()));
    connect(ui->buttonSoftStop, SIGNAL(clicked()), this, SLOT(softStopClicked()));
}

OutputStepperFrame::~OutputStepperFrame()
{
    m_hw->unregisterOutputListener(this);

    delete ui;
}

void OutputStepperFrame::detailsClicked()
{
    OutputStepperDetailsDialog* dialog = new OutputStepperDetailsDialog(m_hw, this);

    dialog->exec();

    delete dialog;
}

void OutputStepperFrame::runVelocityClicked()
{
    m_hw->runVelocity();
}

void OutputStepperFrame::softStopClicked()
{
    m_hw->softStop();
}

void OutputStepperFrame::onOutputChangedGUI()
{
    HWOutputStepper::FullStatus fullStatus = m_hw->getFullStatus();

    ui->labelElDef->setText( fullStatus.elDef ? "Yes" : "No" );
    ui->labelMotion->setText( fullStatus.motion ? "Yes" : "No" );
    ui->labelStall->setText( fullStatus.stall ? "Yes" : "No" );

    switch(fullStatus.tinfo)
    {
    case 00:
        ui->labelTinfo->setText("Normal");
        break;
    case 01:
        ui->labelTinfo->setText("Low");
        break;
    case 10:
        ui->labelTinfo->setText("High");
        break;
    case 11:
        ui->labelTinfo->setText("Shutdown");
        break;
    }

    ui->barIhold->setValue( fullStatus.ihold );
    ui->barIrun->setValue( fullStatus.irun );
    ui->barVmin->setValue( fullStatus.vmin );
    ui->barVmax->setValue( fullStatus.vmax );
}
