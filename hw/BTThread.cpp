
#include "hw/BTThread.h"
#include "util/Config.h"
#include "util/Debug.h"

#include <signal.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

static void dummy_handler(int)
{
    // nothing here
}

BTThread::BTThread()
{
    m_bStop = false;
    m_thread = 0;
    m_btaddr = NULL;
    m_socket = -1;

    // specify a dummy handler for SIGUSR1
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = dummy_handler;
    sigaction(SIGUSR1, &sa, NULL);

    // create and run thread
    pthread_create(&m_thread, NULL, BTThread::run_internal, (void*)this);
}

BTThread::~BTThread()
{
    if(m_thread != 0)
        this->kill();
}

void BTThread::kill()
{
    // set stop to true, so the thread should exit soon
    m_mutex.lock();
    m_bStop = true;
    m_mutex.unlock();

    // deliver signal to thread to wake it up
    pthread_kill(m_thread, SIGUSR1);

    pthread_join(m_thread, NULL);
    m_thread = 0;
}

void BTThread::run()
{
    struct sockaddr_l2 addr = { 0 };

    m_socket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    // set the connection parameters (who to connect to)
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(0x1001);
    str2ba(m_btaddr, &addr.l2_bdaddr);
}

void* BTThread::run_internal(void* arg)
{
    BTThread* thread = (BTThread*)arg;
    thread->run();

    return NULL;
}
