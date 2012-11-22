#ifndef HWOUTPUTSTEPPER_H
#define HWOUTPUTSTEPPER_H

#include "hw/HWOutput.h"

// This implementation is built for amis 30624, it will not work for other stepper motor drivers
class HWOutputStepper : public HWOutput
{
public:
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
        unsigned char absolutThreshold;
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
    struct Params
    {
        unsigned char irun;
        unsigned char ihold;
        unsigned char vmax;
        unsigned char vmin;
        bool accShape;
        unsigned char stepMode;
        bool shaft;
        unsigned char acc;
        unsigned char absolutThreshold;
        unsigned char deltaThreshold;

        short securePosition;
        unsigned char fs2StallEnabled;
        unsigned char minSamples;
        bool dc100StallEnable;
        bool PWMJitterEnable;
    };

    HWOutputStepper();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    FullStatus getFullStatus() const { return m_fullStatus;}
    virtual void refreshFullStatus();

    virtual void setPosition(short position);
    virtual void softStop();
    virtual void runVelocity();

protected:
    short m_position;

    FullStatus m_fullStatus;
};

#endif // HWOUTPUTSTEPPER_H
