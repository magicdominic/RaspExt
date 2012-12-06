#ifndef ACTIONOUTPUTSTEPPERPOSITION_H
#define ACTIONOUTPUTSTEPPERPOSITION_H

#include "script/ActionOutputStepper.h"

class ActionOutputStepperPositioning : public ActionOutputStepper
{
public:
    enum PositioningType
    {
        SetPosition = 0,
        SetDualPosition = 1,
        ResetPosition = 2
    };

    ActionOutputStepperPositioning();
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    StepperType getStepperType() const { return Positioning;}

    bool execute(unsigned int step);
    std::string getDescription() const;


    void setPositioningType(PositioningType type) { m_posType = type;}
    PositioningType getPositioningType() const { return m_posType;}

    void setPosition(short position) { m_position = position;}
    short getPosition() const { return m_position;}
    void setDualPosition(short position1, short position2, unsigned char vmin, unsigned char vmax);
    short getDualPosition1() const { return m_position;}
    short getDualPosition2() const { return m_position2;}
    unsigned char getDualVmin() const { return m_vmin;}
    unsigned char getDualVmax() const { return m_vmax;}

private:
    PositioningType m_posType;
    short m_position;
    short m_position2;
    unsigned char m_vmin;
    unsigned char m_vmax;
};

#endif // ACTIONOUTPUTSTEPPERPOSITION_H
