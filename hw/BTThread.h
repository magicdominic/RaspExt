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

/**
 * @brief The BTThread class does the actual communication with the devices on the Bluetooth boarrd.
 * A HWInput or HWOutput object uses an BTThread object to read or write to/from devices on Bluetooth.
 * There is a seperate BTThread object for each Bluetooth board.
 */
class BTThread
{
public:
    BTThread();
    ~BTThread();

    void start();
    void kill();

    static BTThread* load(QDomElement* root);
    QDomElement save(QDomElement* root, QDomDocument* document);

    void addInput(BTI2CPolling* hw, unsigned int freq);
    void removeInput(BTI2CPolling* hw);

    void addOutput(std::function<void (BTThread*)> func);

    void addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port);
    void removeInputPCF8575(HWInput* hw, int slaveAddress);
    void addOutputPCF8575(HWOutput* hw, int slaveAddress, unsigned int port);
    void removeOutputPCF8575(HWOutput* hw, int slaveAddress);

    void addGPInput(HWInputButtonBtGPIO* hw);
    void removeGPInput(HWInputButtonBtGPIO* hw);

    void setName(std::string name) { m_name = name;}
    std::string getName() const { return m_name;}
    void setBTAddr(std::string addr);
    std::string getBTAddr() const { return m_btaddr;}



    // ATTENTION: USE ONLY IN BTTHREAD!!!!
    void sendI2CPackets(BTI2CPacket* packets, unsigned int num);

private:
    struct InputElement
    {
        unsigned int freq;
        timespec time;
        BTI2CPolling* hw;

        bool operator< (const InputElement& rhs)
        {
            return timespecGreaterThan(this->time, rhs.time);
        }

        bool operator == (const InputElement& rhs)
        {
            // return true if hw objects match
            // this is the case if lhs and rhs are actually the same polling object
            return this->hw == rhs.hw;
        }
    };
    struct OutputElement
    {
        std::function<void (BTThread*)> func;
    };

    static void* run_internal(void* arg);
    void run();

    void connectBt();
    void disconnectBt();
    void reconnectBt();

    void readBlocking();

    bool readWait(timespec timeout);
    void packetHandler(char* buffer, unsigned int length);
    unsigned short seqInc() { m_seq = (m_seq + 1) % 0xFF; return m_seq;}
    void send(char* buffer, unsigned int length);
    void sendGPUpdateRequest(unsigned int pinGroup, BTThread*);


    int m_socket;
    pthread_t m_thread;
    std::mutex m_mutex;
    bool m_bStop;
    std::string m_name;
    std::string m_btaddr; // must be in format 11:22:33:44:55:66

    unsigned short m_seq;
    PriorityQueue<InputElement> m_inputQueue;
    std::queue<OutputElement> m_outputQueue;

    std::list<PCF8575Bt*> m_listPCF8575;

    struct GPInput
    {
        HWInputButtonBtGPIO* hw;
        unsigned int pinGroup;
        unsigned int pin;
    };

    std::list<GPInput> m_listGPInput;

    struct PacketSeq
    {
        unsigned char seq;
        std::function<void (BTThread*, BTI2CPacket*)> callbackFunc;

    };

    std::list<PacketSeq> m_listSeq;
};
#endif // BTTHREAD_H
