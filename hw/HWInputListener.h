#ifndef HWINPUTLISTENER_H
#define HWINPUTLISTENER_H

class HWInput;

// Interface fuer InputEvents
class HWInputListener
{
public:
    virtual void onInputChanged(HWInput* hw) = 0; // wird vom Input aufgerufen, falls ein Input-Wert sich veraendert hat
    virtual void onInputDestroy(HWInput* hw) = 0; // wird vom Input aufgeuren, falls das Input-Objekt zerstoert wird
};

#endif // HWINPUTLISTENER_H
