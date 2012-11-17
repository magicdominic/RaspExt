
#include "script/Condition.h"
#include "script/ConditionInput.h"
#include "script/ConditionVariable.h"

Condition* Condition::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("type") == 0)
        {
            if(elem.text().toLower().compare("input") == 0)
                return ConditionInput::load(root);
            else if(elem.text().toLower().compare("variable") == 0)
                return ConditionVariable::load(root);
        }

        elem = elem.nextSiblingElement();
    }

    return NULL;
}

QDomElement Condition::save(QDomElement* root, QDomDocument* document)
{
    QDomElement condition = document->createElement("condition");

    root->appendChild(condition);

    return condition;
}
