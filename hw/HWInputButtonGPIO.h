#ifndef HWINPUTBUTTONGPIO_H
#define HWINPUTBUTTONGPIO_H

#include "hw/HWInputButton.h"

class HWInputButtonGPIO : public HWInputButton
{
public:
    HWInputButtonGPIO();

    bool init(ConfigManager* config);
    void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    bool handleInterrupt();

    int getFileHandle() const { return m_fd;}

private:
    int m_fd;
    int m_pin;
};

#endif // HWINPUTBUTTONGPIO_H
