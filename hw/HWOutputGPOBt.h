#ifndef HWOUTPUTGPOBT_H
#define HWOUTPUTGPOBT_H

#include "hw/HWOutputGPO.h"

class BTThread;

/**
 * @brief The HWOutputGPOBt class
 * This implementation is specific for the PCF8575. It uses the PCF8575Bt class to communicate over i2c with its chip.
 */
class HWOutputGPOBt : public HWOutputGPO
{
public:
    HWOutputGPOBt();

    virtual void init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    unsigned int getPort() const { return m_port;}
private:
    int m_slaveAddress;
    unsigned int m_port;
    std::string m_btName;
};

#endif // HWOUTPUTGPOBT_H
