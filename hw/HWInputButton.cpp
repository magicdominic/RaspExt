
#include "hw/HWInputButton.h"
#include "hw/HWInputButtonGPIO.h"
#include "hw/HWInputButtonI2C.h"
#include "hw/HWInputButtonBtGPIO.h"
#include "hw/HWInputButtonBt.h"

#include "util/Debug.h"

#include <QDomElement>

HWInputButton::HWInputButton()
{
    m_value = false;
}

HWInput* HWInputButton::load(QDomElement* root)
{
    HWInputButton* hw = NULL;
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("hwtype") == 0)
        {
            if(elem.text().toLower().compare("gpio") == 0)
            {
                hw = (HWInputButton*)HWInputButtonGPIO::load(root);
            }
            else if(elem.text().toLower().compare("i2c") == 0)
            {
                hw = (HWInputButton*)HWInputButtonI2C::load(root);
            }
            else if(elem.text().toLower().compare("btgpio") == 0)
            {
                hw = (HWInputButton*)HWInputButtonBtGPIO::load(root);
            }
            else if(elem.text().toLower().compare("bt") == 0)
            {
                hw = (HWInputButton*)HWInputButtonBt::load(root);
            }
        }
        elem = elem.nextSiblingElement();
    }

    if(hw == NULL)
        hw =  new HWInputButton();

    return hw;
}

QDomElement HWInputButton::save(QDomElement* root, QDomDocument* document)
{
    QDomElement input = HWInput::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("Button");
    type.appendChild(typeText);

    input.appendChild(type);

    return input;
}

bool HWInputButton::setOverrideValue(bool v)
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

bool HWInputButton::getValue() const
{
    return m_value;
}
