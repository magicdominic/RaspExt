#ifndef HWINPUTBUTTONBTGPIO_H
#define HWINPUTBUTTONBTGPIO_H

#include "hw/HWInputButton.h"

class BTThread;

// TODO: maybe the name has to be changed due to a collision with the PCF8575
class HWInputButtonBtGPIO : public HWInputButton
{
public:
    HWInputButtonBtGPIO();

    HWType getHWType() const { return Bt;}

    bool init(ConfigManager* config);
    void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void setValue(bool value);

    unsigned int getPin() { return m_pin;}
    void setPin(unsigned int pin) { m_pin = pin;}
    unsigned int getPinGroup() { return m_pinGroup;}
    void setPinGroup(unsigned int pinGroup) { m_pinGroup = pinGroup;}

    std::string getBTName() const { return m_btName;}
    void setBTName(std::string name) { m_btName = name;}
private:
    unsigned int m_pinGroup;
    unsigned int m_pin;
    std::string m_btName;

    BTThread* m_btThread;
};

#endif // HWINPUTBUTTONBT_H
