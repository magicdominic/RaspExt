#ifndef INPUTBUTTONFRAME_H
#define INPUTBUTTONFRAME_H

#include "ui/InputFrame.h"

#include "hw/HWInputButton.h"


namespace Ui {
    class InputButtonFrame;
}

class InputButtonFrame : public InputFrame
{
    Q_OBJECT
public:
    InputButtonFrame(HWInputButton* hw);
    ~InputButtonFrame();


private slots:
    void overrideClicked();
    void toggleClicked();
    void onInputChangedGUI();

private:
    HWInputButton* m_hw;
    Ui::InputButtonFrame* ui;
};

#endif // INPUTBUTTONFRAME_H
