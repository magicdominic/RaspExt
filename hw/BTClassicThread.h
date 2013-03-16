#ifndef BTCLASSICTHREAD_H
#define BTCLASSICTHREAD_H

#include "hw/BTThread.h"

/**
 * @brief The BTThread class does the actual communication with the devices on the Bluetooth boarrd.
 * A HWInput or HWOutput object uses an BTThread object to read or write to/from devices on Bluetooth.
 * There is a seperate BTThread object for each Bluetooth board.
 */
class BTClassicThread : public BTThread
{
public:
    BTClassicThread();
    ~BTClassicThread();

    void start();
    void kill();

    static BTThread* load(QDomElement* root);

    void addInput(BTI2CPolling* hw, unsigned int freq);
    void removeInput(BTI2CPolling* hw);

    void addOutput(std::function<void (BTThread*)> func);

    void addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port);
    void removeInputPCF8575(HWInput* hw, int slaveAddress);
    void addOutputPCF8575(HWOutput* hw, int slaveAddress, unsigned int port);
    void removeOutputPCF8575(HWOutput* hw, int slaveAddress);

    void addGPInput(HWInputButtonBtGPIO* hw);
    void removeGPInput(HWInputButtonBtGPIO* hw);

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
#endif // BTCLASSICTHREAD_H
