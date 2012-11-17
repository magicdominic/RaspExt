
#include "hw/HWOutput.h"
#include "hw/HWOutputListener.h"

#include "hw/HWOutputLED.h"
#include "hw/HWOutputRelais.h"
#include "hw/HWOutputDCMotor.h"
#include "hw/HWOutputStepper.h"

#include "util/Debug.h"

#include <QDomDocument>

HWOutput::HWOutput()
{
    this->m_bOverride = false;
}

HWOutput::~HWOutput()
{
    for(std::list<HWOutputListener*>::iterator it = m_listListeners.begin(); it != m_listListeners.end(); it++)
    {
        (*it)->onOutputDestroy(this);
    }
}

void HWOutput::init(ConfigManager *config)
{

}

void HWOutput::deinit()
{

}

HWOutput* HWOutput::load(QDomElement *root)
{
    HWOutput* hw = NULL;
    std::string name;
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("type") == 0)
        {
            HWOutputType type = StringToHWOutputType(elem.text().toStdString());
            switch(type)
            {
            case Relais:
                hw = HWOutputRelais::load(root);
                break;
            case DCMotor:
                hw = HWOutputDCMotor::load(root);
                break;
            case LED:
                hw = HWOutputLED::load(root);
                break;
            case Stepper:
                hw = HWOutputStepper::load(root);
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
            pi_warn("Could not load output, name was empty");
            delete hw;
            return NULL;
        }

        hw->setName(name);
    }

    return hw;
}

QDomElement HWOutput::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = document->createElement("output");

    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode(this->getName().c_str());
    name.appendChild(nameText);

    output.appendChild(name);

    root->appendChild(output);

    return output;
}

void HWOutput::setOverride(bool b)
{
    this->m_bOverride = b;
}

bool HWOutput::getOverride() const
{
    return this->m_bOverride;
}

void HWOutput::setName(std::string str)
{
    this->m_name = str;
}

std::string HWOutput::getName() const
{
    return this->m_name;
}

void HWOutput::registerOutputListener(HWOutputListener *listener)
{
    // TODO: check if already registered

    m_listListeners.push_back(listener);

    listener->onOutputChanged(this);
}

void HWOutput::unregisterOutputListener(HWOutputListener *listener)
{
    // TODO: check if registered, otherwise raise error

    m_listListeners.remove(listener);
}

void HWOutput::outputChanged()
{
    for(std::list<HWOutputListener*>::iterator it = m_listListeners.begin(); it != m_listListeners.end(); it++)
    {
        (*it)->onOutputChanged(this);
    }
}

std::string HWOutput::HWOutputTypeToString(HWOutputType type)
{
    switch(type)
    {
    case Relais:
        return "Relais";
    case DCMotor:
        return "DCMotor";
    case LED:
        return "LED";
    case Stepper:
        return "Stepper";
    default:
        pi_warn("Invalid type");
        return "";
    }
}

HWOutput::HWOutputType HWOutput::StringToHWOutputType(std::string str)
{
    const char* cstr = str.c_str();
    if( strcasecmp(cstr, "relais") == 0)
        return Relais;
    else if( strcasecmp(cstr, "dcmotor") == 0)
        return DCMotor;
    else if( strcasecmp(cstr, "led") == 0)
        return LED;
    else if( strcasecmp(cstr, "stepper") == 0)
        return Stepper;
    else
        return EINVALID;
}
