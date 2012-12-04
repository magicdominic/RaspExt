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
class HWInputButtonBt;
class QDomElement;
class QDomDocument;

struct BTI2CPacket
{
    int slaveAddress; // I2C slave address
    bool read; // true for read, false for write
    bool request; // true if it is a request, false if it is a response
    bool error; // true if an error occurred on sending the command sequence over I2C
    unsigned char bufferLength; // buffer length for I2C read
    char* buffer; // pointer to read buffer, should be != NULL for a response (who deletes this?!?)
    unsigned char commandLength; // length of the command sequence
    char* commandSequence; // pointer to buffer containing

    bool assemble(unsigned int size, char* buf);
};

class BTThread;
class BTPolling
{
public:
    virtual void poll(BTThread* btThread) = 0;
    virtual void packetHandler() = 0; // TOODO
};

class BTThread
{
public:
    BTThread();
    ~BTThread();

    void start();
    void kill();

    static BTThread* load(QDomElement* root);
    QDomElement save(QDomElement* root, QDomDocument* document);
/*
    void addInput(I2CPolling* hw, unsigned int freq);
    void removeInput(I2CPolling* hw);

    void addOutput(I2COutput* hw);

    void addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port);
    void removeInputPCF8575(HWInput* hw, int slaveAddress);*/
    void addOutput(std::function<void (BTThread*)> func);

    void addGPInput(HWInputButtonBt* hw);
    void removeGPInput(HWInputButtonBt* hw);

    void setName(std::string name) { m_name = name;}
    std::string getName() const { return m_name;}
    void setBTAddr(std::string addr);
    std::string getBTAddr() const { return m_btaddr;}


    // TODO
    void sendGPUpdateRequest(unsigned int pinGroup, BTThread*);

private:
    struct InputElement
    {
        unsigned int freq;
        timespec time;
        void* hw; // TODO

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

    bool readWait(timespec timeout);
    void packetHandler(char* buffer, unsigned int length);


    int m_socket;
    pthread_t m_thread;
    std::mutex m_mutex;
    bool m_bStop;
    std::string m_name;
    std::string m_btaddr; // must be in format 11:22:33:44:55:66

    PriorityQueue<InputElement> m_inputQueue;
    std::queue<OutputElement> m_outputQueue;

    //std::list<PCF8575I2C*> m_listPCF8575;

    struct GPInput
    {
        HWInputButtonBt* hw;
        unsigned int pinGroup;
        unsigned int pin;
    };

    std::list<GPInput> m_listGPInput;
};
#endif // BTTHREAD_H
