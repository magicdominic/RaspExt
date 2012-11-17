
#include "script/ActionOutputStepper.h"
#include "script/ActionOutputStepperSoftStop.h"
#include "script/ActionOutputStepperRunVelocity.h"
#include "util/Debug.h"

Action* ActionOutputStepper::load(QDomElement* root)
{
    QDomElement elem = root->firstChildElement();
    ActionOutputStepper* action = NULL;

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("steppertype") == 0)
        {
            if(elem.text().toLower().compare("softstop") == 0)
                action = (ActionOutputStepper*)ActionOutputStepperSoftStop::load(root);
            else if(elem.text().toLower().compare("runvelocity") == 0)
                    action = (ActionOutputStepper*)ActionOutputStepperRunVelocity::load(root);
        }

        elem = elem.nextSiblingElement();
    }

    return action;
}

QDomElement ActionOutputStepper::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = ActionOutput::save(root, document);

    QDomElement subtype = document->createElement("subtype");
    QDomText subtypeText = document->createTextNode("Stepper");
    subtype.appendChild(subtypeText);

    action.appendChild(subtype);

    return action;
}
