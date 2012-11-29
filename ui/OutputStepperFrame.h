#ifndef OUTPUTSTEPPERFRAME_H
#define OUTPUTSTEPPERFRAME_H

#include "ui/OutputFrame.h"

#include "hw/HWOutputStepper.h"

namespace Ui {
    class OutputStepperFrame;
}

class OutputStepperFrame : public OutputFrame
{
    Q_OBJECT
public:
    OutputStepperFrame(HWOutputStepper* hw);
    ~OutputStepperFrame();

private slots:
    void detailsClicked();
    void runVelocityClicked();
    void softStopClicked();
    void onOutputChangedGUI();

private:
    HWOutputStepper* m_hw;
    Ui::OutputStepperFrame* ui;
};

#endif // OUTPUTSTEPPERFRAME_H
