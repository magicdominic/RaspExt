#ifndef GPIOINTERRUPTTHREAD_H
#define GPIOINTERRUPTTHREAD_H

#include <pthread.h>

class HWInputButtonGPIO;

class GPIOInterruptThread
{
public:
    GPIOInterruptThread();
    ~GPIOInterruptThread();

    void addGPIOInterrupt(HWInputButtonGPIO* hw);
    void removeGPIOInterrupt(HWInputButtonGPIO *hw);

    void kill();

private:
    static void* run_internal(void* arg);

    void run();

    pthread_t m_thread;
    pthread_mutex_t m_mutex;
    bool m_bStop;

    int m_epfd;
};

#endif // GPIOINTERRUPTTHREAD_H
