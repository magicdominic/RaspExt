
#include "script/Action.h"
#include "script/ActionOutput.h"
#include "script/ActionVariable.h"
#include "script/ActionSleep.h"
#include "script/ActionCallRule.h"
#include "script/ActionMusic.h"

Action* Action::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("type") == 0)
        {
            if(elem.text().toLower().compare("output") == 0)
                return ActionOutput::load(root);
            else if(elem.text().toLower().compare("variable") == 0)
                return ActionVariable::load(root);
            else if(elem.text().toLower().compare("sleep") == 0)
                return ActionSleep::load(root);
            else if(elem.text().toLower().compare("callrule") == 0)
                return ActionCallRule::load(root);
            else if(elem.text().toLower().compare("music") == 0)
                return ActionMusic::load(root);
        }

        elem = elem.nextSiblingElement();
    }

    return NULL;
}

QDomElement Action::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = document->createElement("action");

    root->appendChild(action);

    return action;
}
