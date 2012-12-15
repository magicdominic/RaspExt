
#include "hw/HWOutputRelay.h"
#include "hw/HWOutputRelayI2C.h"
#include "hw/HWOutputRelayBt.h"

#include "util/Debug.h"

#include <QDomElement>

HWOutputRelay::HWOutputRelay()
{
    m_type = Relay;
    m_value = false;
}

HWOutput* HWOutputRelay::load(QDomElement* root)
{
    HWOutputRelay* hw = NULL;
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("hwtype") == 0)
        {
            if(elem.text().toLower().compare("i2c") == 0)
            {
                hw = (HWOutputRelay*)HWOutputRelayI2C::load(root);
            }
            else if(elem.text().toLower().compare("bt") == 0)
            {
                hw = (HWOutputRelay*)HWOutputRelayBt::load(root);
            }
        }
        elem = elem.nextSiblingElement();
    }

    if(hw == NULL)
        hw =  new HWOutputRelay();

    return hw;
}

QDomElement HWOutputRelay::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = HWOutput::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("Relay");
    type.appendChild(typeText);

    output.appendChild(type);

    return output;
}

bool HWOutputRelay::getValue() const
{
    return m_value;
}

void HWOutputRelay::setValue(bool v)
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

bool HWOutputRelay::setOverrideValue(bool v)
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
