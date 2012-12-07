
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
    if(!this->m_bOverride)
        return false;

    if(v != m_value)
    {
        m_value = v;
        this->inputChanged();
    }

    return true;
}

unsigned int HWInputFader::getValue() const
{
    return m_value;
}
