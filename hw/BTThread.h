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
class QDomElement;
class QDomDocument;

class BTThread
{
public:
    BTThread();
    ~BTThread();

    void kill();

    static BTThread* load(QDomElement* root);
    QDomElement save(QDomElement* root, QDomDocument* document);
/*
    void addInput(I2CPolling* hw, unsigned int freq);
    void removeInput(I2CPolling* hw);

    void addOutput(I2COutput* hw);
    void addOutput(std::function<void(int)> func);

    void addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port);
    void removeInputPCF8575(HWInput* hw, int slaveAddress);*/

    void setName(std::string name) { m_name = name;}
    std::string getName() const { return m_name;}
    void setBTAddr(std::string addr);
    std::string getBTAddr() const { return m_btaddr;}

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
        std::function<void (int)> func;
    };

    static void* run_internal(void* arg);
    void run();

    int m_socket;
    pthread_t m_thread;
    std::mutex m_mutex;
    bool m_bStop;
    std::string m_name;
    std::string m_btaddr; // must be in format 11:22:33:44:55:66

    PriorityQueue<InputElement> m_inputQueue;
    std::queue<OutputElement> m_outputQueue;

    //std::list<PCF8575I2C*> m_listPCF8575;
};
#endif // BTTHREAD_H
