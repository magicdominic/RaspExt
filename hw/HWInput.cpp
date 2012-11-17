
#include "hw/HWInput.h"
#include "hw/HWInputListener.h"

#include "hw/HWInputButton.h"
#include "hw/HWInputFader.h"

#include "util/Debug.h"

#include <QDomDocument>

HWInput::HWInput()
{
    this->m_bOverride = false;
}

HWInput::~HWInput()
{
    for(std::list<HWInputListener*>::iterator it = m_listListeners.begin(); it != m_listListeners.end(); it++)
    {
        (*it)->onInputDestroy(this);
    }
}

bool HWInput::init(ConfigManager *config)
{
    return true;
}

void HWInput::deinit(ConfigManager *config)
{

}

HWInput* HWInput::load(QDomElement *root)
{
    QDomElement elem = root->firstChildElement();

    HWInput* hw = NULL;
    std::string name;

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("type") == 0)
        {
            pi_assert(hw == NULL);

            HWInputType type = StringToHWInputType(elem.text().toStdString());

            switch(type)
            {
            case Button:
                hw = HWInputButton::load(root);
                break;
            case Fader:
                hw = HWInputFader::load(root);
                break;
            }
        }
        else if(elem.tagName().toLower().compare("name") == 0)
        {
            name = elem.text().toStdString();
        }

        elem = elem.nextSiblingElement();
    }

    if(hw != NULL)
    {
        if(name.empty())
        {
            pi_warn("Could not create new input, name was empty");

            delete hw;
            return NULL;
        }

        hw->setName(name);
    }

    return hw;
}

QDomElement HWInput::save(QDomElement *root, QDomDocument *document)
{
    QDomElement input = document->createElement("input");

    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode(this->getName().c_str());
    name.appendChild(nameText);

    input.appendChild(name);

    root->appendChild(input);

    return input;
}

void HWInput::setOverride(bool b)
{
    this->m_bOverride = b;
}

bool HWInput::getOverride() const
{
    return this->m_bOverride;
}

void HWInput::setName(std::string str)
{
    this->m_name = str;
}

std::string HWInput::getName() const
{
    return this->m_name;
}

void HWInput::registerInputListener(HWInputListener *listener)
{
    // TODO: check if already registered

    m_listListeners.push_back(listener);

    listener->onInputChanged(this);
}

void HWInput::unregisterInputListener(HWInputListener *listener)
{
    // TODO: check if registered, otherwise raise error

    m_listListeners.remove(listener);
}

void HWInput::inputChanged()
{
    for(std::list<HWInputListener*>::iterator it = m_listListeners.begin(); it != m_listListeners.end(); it++)
    {
        (*it)->onInputChanged(this);
    }
}

std::string HWInput::HWInputTypeToString(HWInputType type)
{
    switch(type)
    {
    case Button:
        return "Button";
        break;
    case Fader:
        return "Fader";
        break;
    default:
        pi_warn("Invalid type");
        return "";
        break;
    }
}

HWInput::HWInputType HWInput::StringToHWInputType(std::string str)
{
    const char* cstr = str.c_str();
    if( strcasecmp(cstr, "button") == 0)
        return Button;
    else if( strcasecmp(cstr, "fader") == 0)
        return Fader;
    else
        return EINVALID;
}
