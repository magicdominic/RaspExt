#ifndef BTTHREAD_H
#define BTTHREAD_H

#include <mutex>
#include <pthread.h>
#include <queue>
#include <list>
#include <functional>

#include "util/Time.h"
#include "util/PriorityQueue.h"

class HWInput;
class HWInputButtonBtGPIO;
class HWOutput;
class BTThread;
class PCF8575Bt;
class QDomElement;
class QDomDocument;


enum BTPacketType
{
    I2C = 0,
    GPIO = 1
};

/**
 * @brief The BTI2CPacket class is used to represent an I2C Bluetooth packet.
 * This class can also be used to assemble and parse an actual packet.
 */
class BTI2CPacket
{
public:
    BTI2CPacket();
    ~BTI2CPacket();
    void assemble(char* buf, unsigned int size);
    unsigned int size() const;
    bool parse(char* buf, unsigned int size);

    int slaveAddress; // I2C slave address
    bool read; // true for read, false for write
    bool request; // true if it is a request, false if it is a response
    bool error; // true if an error occurred on sending the command sequence over I2C

    unsigned char commandLength; // length of the command sequence
    char* commandBuffer; // pointer to buffer containing command sequence

    // only for i2c read response
    unsigned char readLength; // buffer length for I2C read
    char* readBuffer; // pointer to read buffer, should be != NULL for a response

    std::function<void (BTThread*, BTI2CPacket*)> callbackFunc; // used for processing errors and read responses
};

/**
 * @brief The BTI2CPolling class is an interface which all HWInput classes which use Bluetooth implement.
 * The method poll is then used by the BTThread.
 */
class BTI2CPolling
{
public:
    virtual void poll(BTThread* btThread) = 0;
};

class BTThread
{
public:
    BTThread();
    virtual ~BTThread();

    virtual void start() = 0;
    virtual void kill() = 0;

    static BTThread* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    virtual void addInput(BTI2CPolling* hw, unsigned int freq) = 0;
    virtual void removeInput(BTI2CPolling* hw) = 0;

    virtual void addOutput(std::function<void (BTThread*)> func) = 0;

    virtual void addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port) = 0;
    virtual void removeInputPCF8575(HWInput* hw, int slaveAddress) = 0;
    virtual void addOutputPCF8575(HWOutput* hw, int slaveAddress, unsigned int port) = 0;
    virtual void removeOutputPCF8575(HWOutput* hw, int slaveAddress) = 0;

    virtual void addGPInput(HWInputButtonBtGPIO* hw) = 0;
    virtual void removeGPInput(HWInputButtonBtGPIO* hw) = 0;

    void setName(std::string name) { m_name = name;}
    std::string getName() const { return m_name;}
    void setBTAddr(std::string addr);
    std::string getBTAddr() const { return m_btaddr;}


    // ATTENTION: USE ONLY IN BTTHREAD!!!!
    virtual void sendI2CPackets(BTI2CPacket* packets, unsigned int num) = 0;

protected:
    pthread_t m_thread;
    std::mutex m_mutex;
    bool m_bStop;
    std::string m_name;
    std::string m_btaddr; // must be in format 11:22:33:44:55:66
};

#endif // BTTHREAD_H
