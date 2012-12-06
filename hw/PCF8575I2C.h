#ifndef I2CPCF8575_H
#define I2CPCF8575_H

#include "hw/I2CPolling.h"
#include "hw/I2COutput.h"
#include "hw/HWOutputListener.h"

#include <list>

class HWInputButtonI2C;
class HWOutputGPO;
class I2CThread;

class PCF8575I2C : public I2CPolling, I2COutput, HWOutputListener
{
public:
    PCF8575I2C(int slaveAddress);

    void addInput(HWInputButtonI2C* hw, unsigned int port);
    void removeInput(HWInputButtonI2C* hw);

    void addOutput(HWOutputGPO* hw, unsigned int port);
    void removeOutput(HWOutputGPO* hw);

    bool empty() const { return m_listInput.empty() && m_listOutput.empty();}

    int getSlaveAddress() const { return m_slaveAddress;}


    void init(I2CThread* thread);
    void deinit();

    void poll(int fd);
    void setI2C(int fd);

private:
    void onOutputChanged(HWOutput *hw);

    struct InputElement
    {
        unsigned int port;
        HWInputButtonI2C* hw;
    };

    struct OutputElement
    {
        unsigned int port;
        HWOutputGPO* hw;
    };

    void updateI2C();

    I2CThread* m_i2cThread;

    unsigned short m_portMask;
    int m_slaveAddress;

    std::list<InputElement> m_listInput;
    std::list<OutputElement> m_listOutput;
};


#endif // I2CPCF8575_H
