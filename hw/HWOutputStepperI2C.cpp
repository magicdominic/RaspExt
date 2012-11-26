
#include "hw/HWOutputStepperI2C.h"
#include "hw/I2CThread.h"
#include "ConfigManager.h"
#include "util/Debug.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

HWOutputStepperI2C::HWOutputStepperI2C()
{

}

HWOutput* HWOutputStepperI2C::load(QDomElement *root)
{
    HWOutputStepperI2C* hw = new HWOutputStepperI2C();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if( elem.tagName().toLower().compare("slaveaddress") == 0 )
        {
            hw->m_slaveAddress = elem.text().toInt();
        }
        elem = elem.nextSiblingElement();
    }

    // check for invalid parameters
    if( hw->m_slaveAddress > 127 || hw->m_slaveAddress < 0)
    {
        pi_warn("Invalid i2c parameters");
        return NULL;
    }

    return hw;
}

QDomElement HWOutputStepperI2C::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = HWOutputStepper::save(root, document);

    QDomElement type = document->createElement("hwtype");
    QDomText typeText = document->createTextNode("I2C");
    type.appendChild(typeText);

    output.appendChild(type);

    QDomElement slaveAddr = document->createElement("SlaveAddress");
    QDomText slaveAddrText = document->createTextNode(QString::number( m_slaveAddress ));
    slaveAddr.appendChild(slaveAddrText);

    output.appendChild(slaveAddr);

    return output;
}

void HWOutputStepperI2C::init(ConfigManager *config)
{
    m_i2cThread = config->getI2CThread();

    m_i2cThread->addInput(this, 1);
}

void HWOutputStepperI2C::deinit()
{
    m_i2cThread = NULL;
}

void HWOutputStepperI2C::softStop()
{
    HWOutputStepper::softStop();

    m_i2cThread->addOutput(std::bind(&HWOutputStepperI2C::softStopI2C, this, std::placeholders::_1));
}

void HWOutputStepperI2C::setPosition(short position)
{
    HWOutputStepper::setPosition(position);

    m_i2cThread->addOutput(std::bind(&HWOutputStepperI2C::setPositionI2C, this, std::placeholders::_1, position));
}

void HWOutputStepperI2C::runVelocity()
{
    HWOutputStepper::runVelocity();

    m_i2cThread->addOutput(std::bind(&HWOutputStepperI2C::runVelocityI2C, this, std::placeholders::_1));
}

void HWOutputStepperI2C::setParam(Param param)
{
    HWOutputStepper::setParam(param);

    m_i2cThread->addOutput(std::bind(&HWOutputStepperI2C::setParamI2C, this, std::placeholders::_1, param));
}

void HWOutputStepperI2C::refreshFullStatus()
{
    m_i2cThread->addOutput(std::bind(&HWOutputStepperI2C::poll, this, std::placeholders::_1));
}

void HWOutputStepperI2C::poll(int fd)
{
    int ret;
    unsigned char buf[8];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }

    // write GetFullStatus1
    buf[0] = 0x81;

    ret = write(fd, buf, 1);
    if(ret != 1)
    {
        pi_warn("Could not write to bus");
        return;
    }

    // read back value
    ret = read(fd, buf, 8);
    if(ret != 8)
    {
        pi_warn("Could not read from bus");
        return;
    }

    // handle GetFullStatus1
    m_fullStatus.irun = (buf[1] & 0xF0) >> 4;
    m_fullStatus.ihold = buf[1] & 0x0F;
    m_fullStatus.vmax = (buf[2] & 0xF0) >> 4;
    m_fullStatus.vmin = (buf[2] & 0x0F);
    m_fullStatus.accShape = (buf[3] & 0x80) >> 7;
    m_fullStatus.stepMode = (buf[3] & 0x60) >> 5;
    m_fullStatus.shaft = (buf[3] & 0x10) >> 4;
    m_fullStatus.acc = (buf[3] & 0x0F);
    m_fullStatus.vddReset = (buf[4] & 0x80) >> 7;
    m_fullStatus.stepLoss = (buf[4] & 0x40) >> 6;
    m_fullStatus.elDef = (buf[4] & 0x20) >> 5;
    m_fullStatus.uv2 = (buf[4] & 0x10) >> 4;
    m_fullStatus.tsd = (buf[4] & 0x08) >> 3;
    m_fullStatus.tw = (buf[4] & 0x04) >> 2;
    m_fullStatus.tinfo = (buf[4] & 0x03);
    m_fullStatus.motion = (buf[4] & 0xE0) >> 5;
    m_fullStatus.esw = (buf[4] & 0x10) >> 4;
    m_fullStatus.ovc1 = (buf[4] & 0x08) >> 3;
    m_fullStatus.ovc2 = (buf[4] & 0x04) >> 2;
    m_fullStatus.stall = (buf[4] & 0x02) >> 1;
    m_fullStatus.cpfail = (buf[4] & 0x01);
    m_fullStatus.absoluteThreshold = (buf[7] & 0xF0) >> 4;
    m_fullStatus.deltaThreshold = (buf[7] & 0x0F);

    // write GetFullStatus2
    buf[0] = 0xFC;

    ret = write(fd, buf, 1);
    if(ret != 1)
    {
        pi_warn("Could not write to bus");
        return;
    }

    // read back value
    ret = read(fd, buf, 8);
    if(ret != 8)
    {
        pi_warn("Could not read from bus");
        return;
    }

    // handle GetFullStatus2
    m_fullStatus.actualPosition = buf[1] << 8 | buf[2];
    m_fullStatus.targetPosition = buf[3] << 8 | buf[4];
    m_fullStatus.securePosition = (buf[6] & 0x07) << 8 | buf[5];
    m_fullStatus.fs2StallEnabled = (buf[6] & 0xE0) >> 5;
    m_fullStatus.dc100 = (buf[6] & 0x08) >> 3;
    m_fullStatus.absolutStall = (buf[7] & 0x80) >> 7;
    m_fullStatus.deltaStallLow = (buf[7] & 0x40) >> 6;
    m_fullStatus.deltaStallHigh = (buf[7] & 0x20) >> 5;
    m_fullStatus.minSamples = (buf[7] & 0x1C) >> 2;
    m_fullStatus.dc100StallEnable = (buf[7] & 0x02) >> 1;
    m_fullStatus.PWMJitterEnable = (buf[7] & 0x01);

    HWOutputStepper::refreshFullStatus();
}

void HWOutputStepperI2C::softStopI2C(int fd)
{
    int ret;
    unsigned char buf[1];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }
    // send command byte (see page 44 of datasheet)
    buf[0] = 0x8F;

    ret = write(fd, buf, 1);
    if(ret != 1)
    {
        pi_warn("Could not write to bus");
        return;
    }
}

void HWOutputStepperI2C::setPositionI2C(int fd, short position)
{
    int ret;
    unsigned char buf[5];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }

    // send command byte (see page 44 of datasheet)
    buf[0] = 0x8B;
    buf[1] = 0xFF;
    buf[2] = 0xFF;
    buf[3] = (position & 0xFF00) >> 8;
    buf[4] = (position & 0x00FF);

    ret = write(fd, buf, 5);
    if(ret != 5)
    {
        pi_warn("Could not write to bus");
        return;
    }
}

void HWOutputStepperI2C::runVelocityI2C(int fd)
{
    int ret;
    unsigned char buf[1];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }
    // send command byte (see page 44 of datasheet)
    buf[0] = 0x97;

    ret = write(fd, buf, 1);
    if(ret != 1)
    {
        pi_warn("Could not write to bus");
        return;
    }
}

void HWOutputStepperI2C::setParamI2C(int fd, Param param)
{
    // as we need to update EVERY value for these I2C commands,
    // we take the current value from the last FullStatus update and change those specified in param

    int ret;
    unsigned char buf[8];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }

    // send SetStallParam command (see page 49 of datasheet)
    buf[0] = 0x96;
    buf[1] = 0xFF;
    buf[2] = 0xFF;
    buf[3] = (param.irunSet ? param.irun : m_fullStatus.irun) << 4; // irun
    buf[3] = buf[3] | (param.iholdSet ? param.ihold : m_fullStatus.ihold); // ihold
    buf[4] = (param.vmaxSet ? param.vmax : m_fullStatus.vmax) << 4; // vmax
    buf[4] = buf[4] | (param.vminSet ? param.vmin : m_fullStatus.vmin); // vmin
    buf[5] = (param.minSamplesSet ? param.minSamples : m_fullStatus.minSamples) << 5; // minSamples
    buf[5] = buf[5] | ( (param.shaftSet ? param.shaft : m_fullStatus.shaft) << 4 ); // shaft
    buf[5] = buf[5] | (param.accSet ? param.acc : m_fullStatus.acc); // acc
    buf[6] = (param.absoluteThresholdSet ? param.absoluteThreshold : m_fullStatus.absoluteThreshold) << 4; // absoluteThreshold
    buf[6] = buf[6] | (param.deltaThresholdSet ? param.deltaThreshold : m_fullStatus.deltaThreshold); // deltaThreshold
    buf[7] = (param.fs2StallEnabledSet ? param.fs2StallEnabled : m_fullStatus.fs2StallEnabled) << 5; // fs2StallEnabled
    buf[7] = buf[7] | ( (param.accShapeSet ? param.accShape : m_fullStatus.accShape) << 4 ); // accShape
    buf[7] = buf[7] | ( (param.stepModeSet ? param.stepMode : m_fullStatus.stepMode) << 2 ); // stepMode
    buf[7] = buf[7] | ( (param.dc100StallEnableSet ? param.dc100StallEnable : m_fullStatus.dc100StallEnable) << 1 ); // dc100StallEnable
    buf[7] = buf[7] | (param.PWMJitterEnableSet ? param.PWMJitterEnable : m_fullStatus.PWMJitterEnable); // PWMJitterEnable


    ret = write(fd, buf, 8);
    if(ret != 8)
    {
        pi_warn("Could not write to bus");
        return;
    }

    // send SetMotorParam command (see page 49 of datasheet)
    buf[0] = 0x89;
    buf[1] = 0xFF;
    buf[2] = 0xFF;
    buf[3] = (param.irunSet ? param.irun : m_fullStatus.irun) << 4; // irun
    buf[3] = buf[3] | (param.iholdSet ? param.ihold : m_fullStatus.ihold); // ihold
    buf[4] = (param.vmaxSet ? param.vmax : m_fullStatus.vmax) << 4; // vmax
    buf[4] = buf[4] | (param.vminSet ? param.vmin : m_fullStatus.vmin); // vmin
    buf[5] = ( (param.securePositionSet ? param.securePosition : m_fullStatus.securePosition) & 0x0700) << 5; // securePosition[10:8]
    buf[5] = buf[5] | ( (param.shaftSet ? param.shaft : m_fullStatus.shaft) << 4 ); // shaft
    buf[5] = buf[5] | (param.accSet ? param.acc : m_fullStatus.acc); // acc
    buf[6] = (param.securePositionSet ? param.securePosition : m_fullStatus.securePosition) & 0x00FF; // securePosition[7:0]
    buf[7] = 1 << 7 | 1 << 5 | 1 << 1;
    buf[7] = buf[7] | (param.PWMfreqSet ? param.PWMfreq : 0) << 6; // PWMfreq
    buf[7] = buf[7] | ( (param.accShapeSet ? param.accShape : m_fullStatus.accShape) << 4 ); // accShape
    buf[7] = buf[7] | ( (param.stepModeSet ? param.stepMode : m_fullStatus.stepMode) << 2 ); // stepMode
    buf[7] = buf[7] | (param.PWMJitterEnableSet ? param.PWMJitterEnable : m_fullStatus.PWMJitterEnable); // PWMJitterEnable

    ret = write(fd, buf, 8);
    if(ret != 8)
    {
        pi_warn("Could not write to bus");
        return;
    }
}
