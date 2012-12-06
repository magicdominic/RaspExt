
#include "ui/InputFrame.h"
#include "util/Debug.h"


InputFrame::InputFrame(HWInput *hw)
{
    m_hw = hw;
    m_GUIUpdatePending = false;
    QObject::connect(this, SIGNAL(onInputChangedSignal()), this, SLOT(onInputChangedGUI()), Qt::QueuedConnection);
}

void InputFrame::onInputChanged(HWInput *hw)
{
    pi_assert(hw == m_hw);

    // TODO: check if this really can be done without a mutex
    if(!m_GUIUpdatePending)
    {
        m_GUIUpdatePending = true;
        emit onInputChangedSignal();
    }
}

void InputFrame::onInputChangedGUI()
{
    m_GUIUpdatePending = false;
}
