#ifndef HWINPUTFADER_H
#define HWINPUTFADER_H

#include "hw/HWInput.h"

class HWInputFader : public HWInput
{
public:
    HWInputFader();

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    unsigned int getValue() const;
    bool setOverrideValue(unsigned int v);

    HWInputType getType() const { return Fader;}
protected:
    unsigned int m_value;
};

#endif // HWINPUTFADER_H
