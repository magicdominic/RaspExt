
#include "hw/HWInputButtonBt.h"
#include "ConfigManager.h"
#include "hw/BTThread.h"
#include "util/Debug.h"

#include <QDomElement>

HWInputButtonBt::HWInputButtonBt()
{
    this->m_pinGroup = -1;
    this->m_pin = -1;
    m_btThread = NULL;
}

HWInput* HWInputButtonBt::load(QDomElement *root)
{
    HWInputButtonBt* hw = new HWInputButtonBt();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if( elem.tagName().toLower().compare("pingroup") == 0 )
        {
            hw->m_pinGroup = elem.text().toInt();
        }
        else if( elem.tagName().toLower().compare("pin") == 0 )
        {
            hw->m_pin = elem.text().toInt();
        }
        else if( elem.tagName().toLower().compare("btboard") == 0 )
        {
            hw->m_btName = elem.text().toStdString();
        }

        elem = elem.nextSiblingElement();
    }

    // check for invalid parameters
    if( hw->m_pin > 7 || hw->m_pinGroup > 4 || hw->m_btName.empty())
    {
        pi_warn("Invalid bluetooth parameters");
        return NULL;
    }

    return hw;
}

QDomElement HWInputButtonBt::save(QDomElement* root, QDomDocument* document)
{
    QDomElement input = HWInputButton::save(root, document);

    QDomElement type = document->createElement("hwtype");
    QDomText typeText = document->createTextNode("Bt");
    type.appendChild(typeText);

    input.appendChild(type);

    QDomElement pinGroup = document->createElement("PinGroup");
    QDomText pinGroupText = document->createTextNode(QString::number( m_pinGroup ));
    pinGroup.appendChild(pinGroupText);

    input.appendChild(pinGroup);

    QDomElement pin = document->createElement("Pin");
    QDomText pinText = document->createTextNode(QString::number( m_pin ));
    pin.appendChild(pinText);

    input.appendChild(pin);

    QDomElement btBoard = document->createElement("BtBoard");
    QDomText btBoardText = document->createTextNode( QString::fromStdString( m_btName ) );
    btBoard.appendChild(btBoardText);

    input.appendChild(btBoard);

    return input;
}

bool HWInputButtonBt::init(ConfigManager* config)
{
    m_btThread = config->getBTThreadByName(m_btName);

    m_btThread->addGPInput(this);

    return true;
}

void HWInputButtonBt::deinit(ConfigManager* config)
{
    m_btThread->removeGPInput(this);

    m_btThread = NULL;
}

void HWInputButtonBt::setValue(bool value)
{
    // only do something if the value has really changed
    if(m_value != value)
    {
        m_value = value;
        this->inputChanged();
    }
}

void HWInputButtonBt::setOverride(bool v)
{
    HWInput::setOverride(v);
    m_btThread->sendGPUpdateRequest(m_pinGroup, NULL);
}
