#ifndef OUTPUTRELAISFRAME_H
#define OUTPUTRELAISFRAME_H

#include "ui/OutputFrame.h"

#include "hw/HWOutputRelais.h"

namespace Ui {
    class OutputRelaisFrame;
}

class OutputRelaisFrame : public OutputFrame
{
    Q_OBJECT
public:
    OutputRelaisFrame(HWOutputRelais* hw);
    ~OutputRelaisFrame();

private slots:
    void overrideClicked();
    void toggleClicked();
    void onOutputChangedGUI();

private:
    HWOutputRelais* m_hw;
    Ui::OutputRelaisFrame* ui;
};

#endif // OUTPUTRELAISFRAME_H
