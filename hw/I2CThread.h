#ifndef I2CTHREAD_H
#define I2CTHREAD_H

#include <mutex>
#include <pthread.h>
#include <queue>
#include <list>
#include <functional>

#include "util/Time.h"
#include "util/PriorityQueue.h"

class HWInput;
class HWOutput;
class PCF8575I2C;
class I2CThread;

class I2CPolling
{
public:
    virtual void poll(I2CThread* i2cThread) = 0;
};

class I2CThread
{
public:
    I2CThread();
    ~I2CThread();

    void kill();

    void addInput(I2CPolling* hw, unsigned int freq);
    void removeInput(I2CPolling* hw);

    void addOutput(std::function<void(I2CThread*)> func);

    void addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port);
    void removeInputPCF8575(HWInput* hw, int slaveAddress);
    PCF8575I2C* addOutputPCF8575(HWOutput* hw, int slaveAddress, unsigned int port);
    void removeOutputPCF8575(HWOutput* hw, int slaveAddress);

    // TODO: ATTENTION!!! USE ONLY IN I2CTHREAD!!!!
    bool setSlaveAddress(int slaveAddress);
    bool write(void* buffer, unsigned int size);
    bool read(void* buffer, unsigned int size);
private:
    struct InputElement
    {
        unsigned int freq;
        timespec time;
        I2CPolling* hw;

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
        std::function<void (I2CThread*)> func;
    };

    static void* run_internal(void* arg);
    void run();

    pthread_t m_thread;
    std::mutex m_mutex;
    bool m_bStop;

    int m_handle;
    PriorityQueue<InputElement> m_inputQueue;
    std::queue<OutputElement> m_outputQueue;

    std::list<PCF8575I2C*> m_listPCF8575;
};

#endif // I2CTHREAD_H
