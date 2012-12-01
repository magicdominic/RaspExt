#ifndef OUTPUTGPOFRAME_H
#define OUTPUTGPOFRAME_H

#include "ui/OutputFrame.h"

#include "hw/HWOutputGPO.h"

namespace Ui {
    class OutputGPOFrame;
}

class OutputGPOFrame : public OutputFrame
{
    Q_OBJECT
public:
    OutputGPOFrame(HWOutputGPO* hw);
    ~OutputGPOFrame();

private slots:
    void overrideClicked();
    void toggleClicked();
    void onOutputChangedGUI();

private:
    HWOutputGPO* m_hw;
    Ui::OutputGPOFrame* ui;
};

#endif // OUTPUTGPOFRAME_H
