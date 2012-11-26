
#include "hw/HWOutputLED.h"
#include "hw/HWOutputLEDI2C.h"

#include "util/Debug.h"

#include <QDomElement>

HWOutputLED::HWOutputLED()
{
    m_type = LED;
    m_value = 0;
}

HWOutput* HWOutputLED::load(QDomElement* root)
{
    HWOutputLED* hw = NULL;
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("hwtype") == 0)
        {
            if(elem.text().toLower().compare("i2c") == 0)
            {
                hw = (HWOutputLEDI2C*)HWOutputLEDI2C::load(root);
            }
        }
        elem = elem.nextSiblingElement();
    }

    if(hw == NULL)
        hw =  new HWOutputLED();

    return hw;
}

QDomElement HWOutputLED::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = HWOutput::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("LED");
    type.appendChild(typeText);

    output.appendChild(type);

    return output;
}

void HWOutputLED::setValue(unsigned int value)
{
    // if we are in override mode, just return
    if(m_bOverride)
        return;

    if(m_value != value)
    {
        m_value = value;
        this->outputChanged();
    }
}

bool HWOutputLED::setOverrideValue(unsigned int value)
{
    if(!m_bOverride)
        return false;

    if(m_value != value)
    {
        m_value = value;
        this->outputChanged();
    }

    return true;
}
