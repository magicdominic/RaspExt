#ifndef HWINPUTBUTTONI2C_H
#define HWINPUTBUTTONI2C_H

#include "hw/HWInputButton.h"

class HWInputButtonI2C : public HWInputButton
{
public:
    HWInputButtonI2C();

    bool init(ConfigManager* config);
    void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void onInputPolled(bool state);

private:
    int m_slaveAddress;
    unsigned int m_port;
};

#endif // HWINPUTBUTTONI2C_H
