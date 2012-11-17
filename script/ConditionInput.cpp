
#include "script/ConditionInput.h"
#include "script/ConditionInputButton.h"
#include "script/ConditionInputFader.h"
#include "ConfigManager.h"
#include "util/Debug.h"

Condition* ConditionInput::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("subtype") == 0)
        {
            // todo: use function
            if(elem.text().toLower().compare("button") == 0)
                return ConditionInputButton::load(root);
            else if(elem.text().toLower().compare("fader") == 0)
                return ConditionInputFader::load(root);
        }

        elem = elem.nextSiblingElement();
    }

    return NULL;
}

QDomElement ConditionInput::save(QDomElement* root, QDomDocument* document)
{
    QDomElement condition = Condition::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("input");
    type.appendChild(typeText);

    condition.appendChild(type);

    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode(QString::fromStdString(this->m_HWName));
    name.appendChild(nameText);

    condition.appendChild(name);

    return condition;
}

void ConditionInput::init(ConfigManager *config)
{
    m_hw = config->getInputByName(m_HWName);

    pi_assert(m_hw != NULL);

    if(m_hw != NULL)
        m_hw->registerInputListener(this);
}

void ConditionInput::deinit()
{
    if(m_hw != NULL)
        m_hw->unregisterInputListener(this);
}
