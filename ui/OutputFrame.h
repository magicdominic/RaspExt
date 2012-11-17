#ifndef OUTPUTFRAME_H
#define OUTPUTFRAME_H

#include <QFrame>

#include "hw/HWOutput.h"
#include "hw/HWOutputListener.h"

class OutputFrame : public QFrame, public HWOutputListener
{
    Q_OBJECT
public:
    OutputFrame(HWOutput* hw);

    HWOutput* getHW() const { return m_hw;}

    void onOutputChanged(HWOutput* hw);
    void onOutputDestroy(HWOutput* hw);

signals:
    void onOutputChangedSignal();

protected slots:
    virtual void onOutputChangedGUI() = 0;

private:
    HWOutput* m_hw;
};


#endif // OUTPUTFRAME_H
