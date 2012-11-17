#ifndef HWOUTPUTRELAIS_H
#define HWOUTPUTRELAIS_H

#include "hw/HWOutput.h"

class HWOutputRelais : public HWOutput
{
public:
    HWOutputRelais();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    bool getValue() const;
    void setValue(bool v);
    bool setOverrideValue(bool v);

private:
    bool m_value;
};

// TODO: copy I2C Implementation from HWOutputLEDI2C

#endif // HWOUTPUTRELAIS_H
