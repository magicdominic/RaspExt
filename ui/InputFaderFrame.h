#ifndef INPUTFADERFRAME_H
#define INPUTFADERFRAME_H

#include "ui/InputFrame.h"

#include "hw/HWInputFader.h"
#include "hw/HWInputListener.h"


namespace Ui {
    class InputFaderFrame;
}

class InputFaderFrame : public InputFrame
{
    Q_OBJECT
public:
    InputFaderFrame(HWInputFader* hw);
    ~InputFaderFrame();

private slots:
    void overrideClicked();
    void sliderMoved(int value);
    void onInputChangedGUI();

private:
    HWInputFader* m_hw;
    Ui::InputFaderFrame* ui;
};

#endif // INPUTFADERFRAME_H
