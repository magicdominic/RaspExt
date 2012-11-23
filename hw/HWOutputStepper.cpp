
#include "hw/HWOutputStepper.h"
#include "hw/HWOutputStepperI2C.h"
#include "util/Debug.h"

#include <QDomElement>

HWOutputStepper::HWOutputStepper()
{
    m_type = Stepper;
}

HWOutput* HWOutputStepper::load(QDomElement* root)
{
    HWOutputStepper* hw = NULL;
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("hwtype") == 0)
        {
            if(elem.text().toLower().compare("i2c") == 0)
            {
                hw = (HWOutputStepper*)HWOutputStepperI2C::load(root);
            }
        }
        elem = elem.nextSiblingElement();
    }

    if(hw == NULL)
        hw =  new HWOutputStepper();

    return hw;
}

QDomElement HWOutputStepper::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = HWOutput::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("Stepper");
    type.appendChild(typeText);

    output.appendChild(type);

    return output;
}

void HWOutputStepper::refreshFullStatus()
{
}

void HWOutputStepper::softStop()
{
}

void HWOutputStepper::setPosition(short position)
{
    m_fullStatus.actualPosition = position;

    this->outputChanged();
}

void HWOutputStepper::runVelocity()
{
}

void HWOutputStepper::setParam(Param param)
{
}



/**
 * @brief HWOutputStepper::Param::Param This constructor is used only to initialize all the important variables to a defined value
 */
HWOutputStepper::Param::Param()
{
    // This could be replaced by memset for more performance
    irunSet = false;
    iholdSet = false;
    vmaxSet = false;
    vminSet = false;
    accShapeSet = false;
    stepModeSet = false;
    shaftSet = false;
    accSet = false;
    absoluteThresholdSet = false;
    deltaThresholdSet = false;
    securePositionSet = false;
    fs2StallEnabledSet = false;
    minSamplesSet = false;
    dc100StallEnableSet = false;
    PWMJitterEnableSet = false;
    PWMfreqSet = false;
}
