#ifndef HWINPUTBUTTONBT_H
#define HWINPUTBUTTONBT_H

#include "hw/HWInputButton.h"

class HWInputButtonBt : public HWInputButton
{
public:
    HWInputButtonBt();

    HWType getHWType() const { return BtI2C;}

    bool init(ConfigManager* config);
    void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void onInputPolled(bool state);

    int getPort() const { return m_port;}
    void setPort(unsigned int port) { m_port = port;}

    int getSlaveAddress() const { return m_slaveAddress;}
    void setSlaveAddress(int slaveAddress) { m_slaveAddress = slaveAddress;}

    std::string getBTName() const { return m_btName;}
    void setBTName(std::string name) { m_btName = name;}
private:
    int m_slaveAddress;
    unsigned int m_port;
    std::string m_btName;
};

#endif // HWINPUTBUTTONBT_H
