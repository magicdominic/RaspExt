
#include "ui/OutputStepperDetailsDialog.h"
#include "ui_OutputStepperDetailsDialog.h"

#include "util/Debug.h"


OutputStepperDetailsDialog::OutputStepperDetailsDialog(HWOutputStepper *hw, QWidget *parent) : QDialog(parent), ui(new Ui::OutputStepperDetailsDialog())
{
    m_hw = hw;

    ui->setupUi(this);
    this->setWindowTitle( QString::fromStdString( m_hw->getName() ) );

    // connect to parent to receive signals
    // These will be used to update our content
    connect(parent, SIGNAL(onOutputChangedSignal()), this, SLOT(onOutputChangedGUI()));
    connect(ui->buttonRefresh, SIGNAL(clicked()), this, SLOT(refreshClicked()));
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(close()));

    // refresh dialog content
    this->refreshClicked();
}

OutputStepperDetailsDialog::~OutputStepperDetailsDialog()
{
    delete ui;
}

void OutputStepperDetailsDialog::onOutputChangedGUI()
{
    HWOutputStepper::FullStatus fullStatus = m_hw->getFullStatus();

    // update every gui element
    ui->labelActPos->setText( QString::number( fullStatus.actualPosition ) );
    ui->labelTagPos->setText( QString::number( fullStatus.targetPosition ) );
    ui->labelSecPos->setText( QString::number( fullStatus.securePosition ) );
    ui->labelAccShape->setText( QString::number( fullStatus.accShape ) );
    ui->labelIrun->setText( QString::number( fullStatus.irun ) );
    ui->labelIhold->setText( QString::number( fullStatus.ihold ) );
    ui->labelVmin->setText( QString::number( fullStatus.vmin ) );
    ui->labelVmax->setText( QString::number( fullStatus.vmax ) );
    ui->labelShaft->setText( QString::number( fullStatus.shaft ) );
    ui->labelAcc->setText( QString::number( fullStatus.acc ) );
    ui->labelStepmode->setText( QString::number( fullStatus.stepMode ) );
    ui->labelAbsThr->setText( QString::number( fullStatus.absoluteThreshold ) );
    ui->labelDelThr->setText( QString::number( fullStatus.deltaThreshold ) );
    ui->labelFS2StEn->setText( QString::number( fullStatus.fs2StallEnabled ) );
    ui->labelMinSamples->setText( QString::number( fullStatus.minSamples ) );
    ui->labelPWMJEn->setText( QString::number( fullStatus.PWMJitterEnable ) );
    ui->labelDC100StEn->setText( QString::number( fullStatus.dc100StallEnable ) );
}

void OutputStepperDetailsDialog::refreshClicked()
{
    m_hw->refreshFullStatus();
}
