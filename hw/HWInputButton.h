#ifndef HWINPUTBUTTON_H
#define HWINPUTBUTTON_H

#include "hw/HWInput.h"

class HWInputButton : public HWInput
{
public:
    HWInputButton();

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    bool getValue() const;
    bool setOverrideValue(bool v);

    HWInputType getType() const { return Button;}
protected:
    bool m_value;
};

#endif // HWINPUTBUTTON_H
