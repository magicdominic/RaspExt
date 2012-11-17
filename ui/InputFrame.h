#ifndef INPUTFRAME_H
#define INPUTFRAME_H

#include <QFrame>

#include "hw/HWInput.h"
#include "hw/HWInputListener.h"

class InputFrame : public QFrame, public HWInputListener
{
    Q_OBJECT
public:
    InputFrame(HWInput* hw);

    HWInput* getHW() const { return m_hw;}

    void onInputChanged(HWInput *hw);
    void onInputDestroy(HWInput *hw);

signals:
    void onInputChangedSignal();

protected slots:
    virtual void onInputChangedGUI();

private:
    HWInput* m_hw;
    bool m_GUIUpdatePending;
};

#endif // INPUTFRAME_H
