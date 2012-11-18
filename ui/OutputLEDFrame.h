#ifndef OUTPUTLEDFRAME_H
#define OUTPUTLEDFRAME_H

#include "ui/OutputFrame.h"

#include "hw/HWOutputLED.h"

namespace Ui {
    class OutputLEDFrame;
}

class OutputLEDFrame : public OutputFrame
{
    Q_OBJECT
public:
    OutputLEDFrame(HWOutputLED* hw);
    ~OutputLEDFrame();

private slots:
    void overrideClicked();
    void sliderChanged(int value);
    void onOutputChangedGUI();

private:
    HWOutputLED* m_hw;
    Ui::OutputLEDFrame* ui;
};

#endif // OUTPUTLEDFRAME_H
