#ifndef HWOUTPUTLEDBT_H
#define HWOUTPUTLEDBT_H

#include "hw/HWOutputLED.h"

class BTThread;

// this implementation is specific for TLC59116, it will not work for other led drivers
class HWOutputLEDBt : public HWOutputLED
{
public:
    HWOutputLEDBt();

    virtual void init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);
private:
    void outputChanged();

    void setI2CBt(BTThread* btThread);
    void setupI2CBt(BTThread* btThread);

    int m_slaveAddress;
    unsigned int m_channel;
    std::string m_btName;
    BTThread* m_btThread;
};

#endif // HWOUTPUTLEDBT_H
