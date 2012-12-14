
#include "hw/HWOutputGPO.h"
#include "hw/HWOutputGPOI2C.h"
#include "hw/HWOutputGPOBt.h"

#include "util/Debug.h"

#include <QDomElement>

HWOutputGPO::HWOutputGPO()
{
    m_type = GPO;
    m_value = false;
}

HWOutput* HWOutputGPO::load(QDomElement* root)
{
    HWOutputGPO* hw = NULL;
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("hwtype") == 0)
        {
            if(elem.text().toLower().compare("i2c") == 0)
            {
                hw = (HWOutputGPO*)HWOutputGPOI2C::load(root);
            }
            else if(elem.text().toLower().compare("bt") == 0)
            {
                hw = (HWOutputGPO*)HWOutputGPOBt::load(root);
            }
        }
        elem = elem.nextSiblingElement();
    }

    if(hw == NULL)
        hw =  new HWOutputGPO();

    return hw;
}

QDomElement HWOutputGPO::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = HWOutput::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("GPO");
    type.appendChild(typeText);

    output.appendChild(type);

    return output;
}

bool HWOutputGPO::getValue() const
{
    return m_value;
}

void HWOutputGPO::setValue(bool v)
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

bool HWOutputGPO::setOverrideValue(bool v)
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
