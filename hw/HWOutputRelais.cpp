
#include "hw/HWOutputRelais.h"

#include "util/Debug.h"

#include <QDomElement>

HWOutputRelais::HWOutputRelais()
{
    m_type = Relais;
    m_value = false;
}

HWOutput* HWOutputRelais::load(QDomElement* root)
{
    HWOutputRelais* hw = new HWOutputRelais();

    return hw;
}

QDomElement HWOutputRelais::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = HWOutput::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("Relais");
    type.appendChild(typeText);

    output.appendChild(type);

    return output;
}

bool HWOutputRelais::getValue() const
{
    return m_value;
}

void HWOutputRelais::setValue(bool v)
{
    // if we are in override mode, just return
    if(m_bOverride)
        return;

    if(m_value != v)
    {
        m_value = v;
        this->outputChanged();
    }
}

bool HWOutputRelais::setOverrideValue(bool v)
{
    if(!m_bOverride)
        return false;

    if(m_value != v)
    {
        m_value = v;
        this->outputChanged();
    }

    return true;
}
