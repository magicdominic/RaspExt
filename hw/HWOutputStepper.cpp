
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
    this->outputChanged();
}

void HWOutputStepper::testBemf()
{
}

void HWOutputStepper::softStop()
{
}

void HWOutputStepper::setPosition(short position)
{
    m_fullStatus.targetPosition = position;

    this->outputChanged();
}

void HWOutputStepper::setDualPosition(short position1, short position2, unsigned char vmin, unsigned char vmax)
{
    this->outputChanged();
}

void HWOutputStepper::resetPosition()
{
    m_fullStatus.actualPosition = 0;

    this->outputChanged();
}

void HWOutputStepper::runVelocity()
{
}

void HWOutputStepper::setParam(Param param)
{
    // update FullStatus
    // we do this now to eliminate a gap between setting the values and displaying them
    // If our new values are not accepted we have an inconsistency between displayed and real values, so we can only hope that this does not happen much
    if(param.irunSet) m_fullStatus.irun = param.irun;
    if(param.iholdSet) m_fullStatus.ihold = param.ihold;
    if(param.vmaxSet) m_fullStatus.vmax = param.vmax;
    if(param.vminSet) m_fullStatus.vmin = param.vmin;
    if(param.accShapeSet) m_fullStatus.accShape = param.accShape;
    if(param.stepModeSet) m_fullStatus.stepMode = param.stepMode;
    if(param.shaftSet) m_fullStatus.shaft = param.shaft;
    if(param.accSet) m_fullStatus.acc = param.acc;
    if(param.absoluteThresholdSet) m_fullStatus.absoluteThreshold = param.absoluteThreshold;
    if(param.deltaThresholdSet) m_fullStatus.deltaThreshold = param.deltaThreshold;
    if(param.securePositionSet) m_fullStatus.securePosition = param.securePosition;
    if(param.fs2StallEnabledSet) m_fullStatus.fs2StallEnabled = param.fs2StallEnabled;
    if(param.minSamplesSet) m_fullStatus.minSamples = param.minSamples;
    if(param.dc100StallEnableSet) m_fullStatus.dc100StallEnable = param.dc100StallEnable;
    if(param.PWMJitterEnableSet) m_fullStatus.PWMJitterEnable = param.PWMJitterEnable;

    this->outputChanged();
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
