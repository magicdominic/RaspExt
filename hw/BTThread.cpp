
#include "hw/BTThread.h"
#include "util/Config.h"
#include "util/Debug.h"

#include <signal.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

#include <QDomDocument>

static void dummy_handler(int)
{
    // nothing here
}

BTThread::BTThread()
{
    m_bStop = false;
    m_thread = 0;
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

BTThread* BTThread::load(QDomElement* root)
{
    BTThread* btthread = new BTThread();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("name") == 0)
        {
            btthread->setName( elem.text().toStdString() );
        }
        else if(elem.tagName().toLower().compare("btaddress") == 0)
        {
            btthread->setBTAddr( elem.text().toStdString() );
        }

        elem = elem.nextSiblingElement();
    }

    if(btthread->getName().empty() || btthread->getBTAddr().empty())
    {
        pi_warn("Could not load bluetooth, name and/or address for this module were empty");
        delete btthread;
    }

    return btthread;
}

QDomElement BTThread::save(QDomElement* root, QDomDocument* document)
{
    QDomElement output = document->createElement("bluetooth");

    QDomElement name = document->createElement("name");
    QDomText nameText = document->createTextNode( QString::fromStdString( this->getName() ) );
    name.appendChild(nameText);

    output.appendChild(name);

    QDomElement btaddr = document->createElement("BTAddress");
    QDomText btaddrText = document->createTextNode( QString::fromStdString( this->getBTAddr() ) );
    btaddr.appendChild(btaddrText);

    output.appendChild(btaddr);

    root->appendChild(output);

    return output;
}

void BTThread::setBTAddr(std::string addr)
{
    // TODO: check format and only save it is correct
    m_btaddr = addr;
}

void BTThread::run()
{
    struct sockaddr_l2 addr;

    m_socket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    if(m_socket == -1)
    {
        pi_warn("Could not open bluetooth socket");
        return;
    }

    // set the connection parameters (who to connect to)
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(0x1001);
    str2ba(m_btaddr.c_str(), &addr.l2_bdaddr);


    // maybe wo should do this in the run loop, as we may loose our connection to the board or it is not yet available
    int status = connect(m_socket, (struct sockaddr*)&addr, sizeof(addr));
    if(status == -1)
    {
        pi_warn("Could not connect to bt-board");
        return;
    }
}

void* BTThread::run_internal(void* arg)
{
    BTThread* thread = (BTThread*)arg;
    thread->run();

    return NULL;
}
