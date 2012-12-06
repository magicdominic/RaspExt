#ifndef HWOUTPUTSTEPPER_H
#define HWOUTPUTSTEPPER_H

#include "hw/HWOutput.h"

// This implementation is built for amis 30624, it will not work for other stepper motor drivers
class HWOutputStepper : public HWOutput
{
public:
    /** This structure contains the response to GetFullStatus1 and GetFullStatus2 from the stepper motor driver
     * Use HWOutputStepper::getFullStatus() to get the last updated values. If you want to refresh the information contained there,
     * use refreshFullStatus and register as an OutputListener. As soon as an update is available, you will get called.
     */
    struct FullStatus
    {
        // Response to GetFullStatus1
        unsigned char irun;
        unsigned char ihold;
        unsigned char vmax;
        unsigned char vmin;
        bool accShape;
        unsigned char stepMode;
        bool shaft;
        unsigned char acc;
        bool vddReset;
        bool stepLoss;
        bool elDef;
        bool uv2;
        bool tsd;
        bool tw;
        unsigned char tinfo;
        unsigned char motion;
        bool esw;
        bool ovc1;
        bool ovc2;
        bool stall;
        bool cpfail;
        unsigned char absoluteThreshold;
        unsigned char deltaThreshold;

        // Response to GetFullStatus2
        short actualPosition;
        short targetPosition;
        short securePosition;
        unsigned char fs2StallEnabled;
        bool dc100;
        bool absolutStall;
        bool deltaStallLow;
        bool deltaStallHigh;
        unsigned char minSamples;
        bool dc100StallEnable;
        bool PWMJitterEnable;
    };

    /** This structure is used to set specific parameters of the stepper device.
     *  Each value has a corresponding boolean parameter, which specifies if the value on the device should be overwritten or kept
     */
    class Param
    {
    public:
        bool irunSet;
        unsigned char irun;

        bool iholdSet;
        unsigned char ihold;

        bool vmaxSet;
        unsigned char vmax;

        bool vminSet;
        unsigned char vmin;

        bool accShapeSet;
        bool accShape;

        bool stepModeSet;
        unsigned char stepMode;

        bool shaftSet;
        bool shaft;

        bool accSet;
        unsigned char acc;

        bool absoluteThresholdSet;
        unsigned char absoluteThreshold;

        bool deltaThresholdSet;
        unsigned char deltaThreshold;

        bool securePositionSet;
        short securePosition;

        bool fs2StallEnabledSet;
        unsigned char fs2StallEnabled;

        bool minSamplesSet;
        unsigned char minSamples;

        bool dc100StallEnableSet;
        bool dc100StallEnable;

        bool PWMJitterEnableSet;
        bool PWMJitterEnable;

        bool PWMfreqSet;
        bool PWMfreq;

        Param();
    };

    HWOutputStepper();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    FullStatus getFullStatus() const { return m_fullStatus;}
    virtual void refreshFullStatus();

    virtual void setPosition(short position);
    virtual void setDualPosition(short position1, short position2, unsigned char vmin, unsigned char vmax);
    virtual void resetPosition();
    virtual void softStop();
    virtual void runVelocity();
    virtual void setParam(Param param);

protected:
    FullStatus m_fullStatus;
};

#endif // HWOUTPUTSTEPPER_H
