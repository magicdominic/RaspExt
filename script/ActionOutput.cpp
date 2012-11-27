#include "script/ActionOutput.h"
#include "script/ActionOutputRelay.h"
#include "script/ActionOutputDCMotor.h"
#include "script/ActionOutputStepper.h"
#include "ConfigManager.h"

#include "util/Debug.h"

Action* ActionOutput::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();
    std::string name;
    ActionOutput* action = NULL;

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("subtype") == 0)
        {
            if(elem.text().toLower().compare("relay") == 0)
                action = (ActionOutput*)ActionOutputRelay::load(root);
            else if(elem.text().toLower().compare("dcmotor") == 0)
                action = (ActionOutput*)ActionOutputDCMotor::load(root);
            else if(elem.text().toLower().compare("stepper") == 0)
                action = (ActionOutput*)ActionOutputStepper::load(root);
        }
        else if(elem.tagName().toLower().compare("name") == 0)
            name = elem.text().toStdString();

        elem = elem.nextSiblingElement();
    }

    if(action != NULL)
    {
        // name cannot be empty
        if( name.empty() )
        {
            delete action;
            return NULL;
        }

        action->setHWName(name);
    }

    return action;
}

QDomElement ActionOutput::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = Action::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("output");
    type.appendChild(typeText);

    action.appendChild(type);

    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode(QString::fromStdString( m_HWName ));
    name.appendChild(nameText);

    action.appendChild(name);

    return action;
}

void ActionOutput::init(ConfigManager *config)
{
    m_hw = config->getOutputByName(m_HWName);
}

void ActionOutput::deinit()
{
    m_hw = NULL;
}
