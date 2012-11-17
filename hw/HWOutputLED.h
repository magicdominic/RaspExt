#ifndef HWOUTPUTLED_H
#define HWOUTPUTLED_H

#include "hw/HWOutput.h"

class HWOutputLED : public HWOutput
{
public:
    HWOutputLED();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    unsigned int getValue() const { return m_value;}
    void setValue(unsigned int value);
    bool setOverrideValue(unsigned int value);

protected:
    unsigned int m_value;
};

#endif // HWOUTPUTLED_H
