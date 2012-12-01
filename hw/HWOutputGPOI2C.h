#ifndef HWOUTPUTGPOI2C_H
#define HWOUTPUTGPOI2C_H

#include "hw/HWOutputGPO.h"

class I2CThread;

/**
 * @brief The HWOutputGPOI2C class
 * This implementation is specific for the PCF8575. It uses the PCF8575I2C class to communicate over i2c with its chip.
 */
class HWOutputGPOI2C : public HWOutputGPO
{
public:
    HWOutputGPOI2C();

    virtual void init(ConfigManager* config);
    virtual void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    unsigned int getPort() const { return m_port;}
private:
    int m_slaveAddress;
    unsigned int m_port;
    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTGPOI2C_H
