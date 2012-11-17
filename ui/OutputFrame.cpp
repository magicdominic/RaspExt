
#include "ui/OutputFrame.h"
#include "util/Debug.h"


OutputFrame::OutputFrame(HWOutput *hw)
{
    m_hw = hw;
    QObject::connect(this, SIGNAL(onOutputChangedSignal()), this, SLOT(onOutputChangedGUI()), Qt::QueuedConnection);
}

void OutputFrame::onOutputChanged(HWOutput *hw)
{
    pi_assert(hw == m_hw);
    emit onOutputChangedSignal();
}

void OutputFrame::onOutputDestroy(HWOutput *hw)
{
    pi_error("HWOutput was destroyed while still in use by GUI");
}
