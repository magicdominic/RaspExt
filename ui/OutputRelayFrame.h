#ifndef OUTPUTRELAYFRAME_H
#define OUTPUTRELAYFRAME_H

#include "ui/OutputFrame.h"

#include "hw/HWOutputRelay.h"

namespace Ui {
    class OutputRelayFrame;
}

class OutputRelayFrame : public OutputFrame
{
    Q_OBJECT
public:
    OutputRelayFrame(HWOutputRelay* hw);
    ~OutputRelayFrame();

private slots:
    void overrideClicked();
    void toggleClicked();
    void onOutputChangedGUI();

private:
    HWOutputRelay* m_hw;
    Ui::OutputRelayFrame* ui;
};

#endif // OUTPUTRELAYFRAME_H
