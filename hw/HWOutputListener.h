#ifndef HWOUTPUTLISTENER_H
#define HWOUTPUTLISTENER_H

class HWOutput;

// Interface fuer InputEvents
class HWOutputListener
{
public:
    virtual void onOutputChanged(HWOutput* hw) = 0; // wird vom Output aufgerufen, falls ein Output-Wert sich veraendert hat
};

#endif // HWOUTPUTLISTENER_H
