#ifndef HWOUTPUTLISTENER_H
#define HWOUTPUTLISTENER_H

class HWOutput;

// Interface fuer InputEvents
class HWOutputListener
{
public:
    virtual void onOutputChanged(HWOutput* hw) = 0; // wird vom Output aufgerufen, falls ein Input-Wert sich veraendert hat
    virtual void onOutputDestroy(HWOutput* hw) = 0; // wird vom Output aufgeuren, falls das Input-Objekt zerstoert wird
};

#endif // HWOUTPUTLISTENER_H
