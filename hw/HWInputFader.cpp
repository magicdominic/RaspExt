
#include "hw/HWInputFader.h"
#include "hw/HWInputFaderI2C.h"
#include "hw/HWInputFaderBt.h"

#include "util/Debug.h"

#include <QDomElement>

HWInputFader::HWInputFader()
{
    m_value = 0;
}

HWInput* HWInputFader::load(QDomElement* root)
{
    HWInputFader* hw = NULL;
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("hwtype") == 0)
        {
            if(elem.text().toLower().compare("i2c") == 0)
            {
                hw = (HWInputFader*)HWInputFaderI2C::load(root);
            }
            else if(elem.text().toLower().compare("bt") == 0)
            {
                hw = (HWInputFader*)HWInputFaderBt::load(root);
            }
        }
        elem = elem.nextSiblingElement();
    }

    if(hw == NULL)
        hw =  new HWInputFader();

    return hw;
}

QDomElement HWInputFader::save(QDomElement *root, QDomDocument *document)
{
    QDomElement input = HWInput::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("Fader");
    type.appendChild(typeText);

    input.appendChild(type);

    return input;
}

bool HWInputFader::setOverrideValue(unsigned int v)
{
    if(!m_bOverride)
        return false;

    if(v != m_value)
    {
        m_value = v;
        this->inputChanged();
    }

    return true;
}

void HWInputFader::setValue(unsigned int value)
{
    if(m_bOverride || m_value == value)
        return;

    // make sure value is valid
    if(value > 100)
        value = 100;

    // ignore small changes due to noise
    unsigned int diff = m_value > value ? (m_value - value) : (value - m_value);
    if( diff > 1 || value == 100 || value == 0)
    {
        m_value = value;
        this->inputChanged();
    }
}

unsigned int HWInputFader::getValue() const
{
    return m_value;
}
