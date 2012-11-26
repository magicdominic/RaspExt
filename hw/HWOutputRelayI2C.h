#ifndef HWOUTPUTRELAYI2C_H
#define HWOUTPUTRELAYI2C_H

#include "hw/HWOutputRelay.h"
#include "hw/I2COutput.h"

class I2CThread;

/** HWOutputRelayI2C is essentially the same as HWOutputLEDI2C, as both use the same chip.
 * However, with the led implementation one can control the individual brightness of each led,
 * whereas with this implementation one can only control if it is fully on or fully of (relay mode).
 * This implementation is specific for TLC59116, it will not work for other led drivers!
 */
class HWOutputRelayI2C : public HWOutputRelay, public I2COutput
{
public:
    HWOutputRelayI2C();

    virtual void init(ConfigManager* config);
    virtual void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void setI2C(int fd);
private:
    void outputChanged();

    void setupI2C(int fd);

    int m_slaveAddress;
    unsigned int m_channel;
    I2CThread* m_i2cThread;
};

#endif // HWOUTPUTRELAYI2C_H
