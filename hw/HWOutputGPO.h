#ifndef HWOUTPUTGPO_H
#define HWOUTPUTGPO_H

#include "hw/HWOutput.h"

class HWOutputGPO : public HWOutput
{
public:
    HWOutputGPO();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    bool getValue() const;
    void setValue(bool v);
    bool setOverrideValue(bool v);

protected:
    bool m_value;
};

#endif // HWOUTPUTGPO_H
