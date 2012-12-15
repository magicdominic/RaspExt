#ifndef HWOUTPUTSTEPPERBT_H
#define HWOUTPUTSTEPPERBT_H

#include "hw/HWOutputStepper.h"
#include "hw/BTThread.h"

// this implementation is specific for AMIS 30624
class HWOutputStepperBt : public HWOutputStepper, public BTI2CPolling
{
public:
    void init(ConfigManager* config);
    void deinit(ConfigManager* config);

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void testBemf();
    void setPosition(short position, bool override);
    void setDualPosition(short position1, short position2, unsigned char vmin, unsigned char vmax, bool override);
    void resetPosition(bool override);
    void softStop(bool override);
    void runVelocity(bool override);
    void setParam(Param param, bool override);
    void refreshFullStatus();
private:
    void poll(BTThread* i2cThread);
    void getFullStatus1Callback(BTThread* btThread, BTI2CPacket* packet);
    void getFullStatus2Callback(BTThread* btThread, BTI2CPacket* packet);

    void testBemfI2C(BTThread* btThread);
    void softStopI2C(BTThread* btThread, bool override);
    void setPositionI2C(BTThread* btThread, short position, bool override);
    void setDualPositionI2C(BTThread* btThread, short position1, short position2, unsigned char vmin, unsigned char vmax, bool override);
    void resetPositionI2C(BTThread* btThread, bool override);
    void runVelocityI2C(BTThread* btThread, bool override);
    void setParamI2C(BTThread* btThread, Param param, bool override);

    std::string m_btName;
    BTThread* m_btThread;
};

#endif // HWOUTPUTSTEPPERBT_H
