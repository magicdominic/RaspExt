#ifndef OUTPUTDCMOTORFRAME_H
#define OUTPUTDCMOTORFRAME_H

#include "ui/OutputFrame.h"

#include "hw/HWOutputDCMotor.h"

namespace Ui {
    class OutputDCMotorFrame;
}

class OutputDCMotorFrame : public OutputFrame
{
    Q_OBJECT
public:
    OutputDCMotorFrame(HWOutputDCMotor* hw);
    ~OutputDCMotorFrame();

private slots:
    void overrideClicked();
    void onOutputChangedGUI();

    void sliderChanged();
    void forwardClicked();
    void reverseClicked();
    void brakeClicked();

private:
    HWOutputDCMotor* m_hw;
    Ui::OutputDCMotorFrame* ui;
};

#endif // OUTPUTDCMOTORFRAME_H
