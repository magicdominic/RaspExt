#ifndef OUTPUTSTEPPERDETAILSDIALOG_H
#define OUTPUTSTEPPERDETAILSDIALOG_H


#include <QDialog>

#include "hw/HWOutputStepper.h"

namespace Ui {
    class OutputStepperDetailsDialog;
}

class OutputStepperDetailsDialog : public QDialog
{
    Q_OBJECT
public:
    OutputStepperDetailsDialog(HWOutputStepper* hw, QWidget* parent);
    ~OutputStepperDetailsDialog();

private slots:
    void testBemf();
    void refreshClicked();
    void onOutputChangedGUI();

private:
    HWOutputStepper* m_hw;
    Ui::OutputStepperDetailsDialog* ui;
};

#endif // OUTPUTSTEPPERDETAILSDIALOG_H
