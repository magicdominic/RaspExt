
#include "script/ActionOutputStepper.h"
#include "script/ActionOutputStepperSoftStop.h"
#include "script/ActionOutputStepperRunVelocity.h"
#include "script/ActionOutputStepperSetPosition.h"
#include "script/ActionOutputStepperSetParam.h"
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
            else if(elem.text().toLower().compare("setposition") == 0)
                    action = (ActionOutputStepper*)ActionOutputStepperSetPosition::load(root);
            else if(elem.text().toLower().compare("setparam") == 0)
                    action = (ActionOutputStepper*)ActionOutputStepperSetParam::load(root);
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


void ActionOutputStepper::getRequiredList(std::list<Rule::RequiredInput>* listInput,
                                     std::list<Rule::RequiredOutput>* listOutput,
                                     std::list<Rule::RequiredVariable>* listVariable) const
{
    if(listOutput != NULL)
    {
        Rule::RequiredOutput req;
        req.name = m_HWName;
        req.type = HWOutput::Stepper;
        listOutput->push_back(req);
    }
}
