#ifndef HWINPUTFADERBT_H
#define HWINPUTFADERBT_H

#include "hw/HWInputFader.h"
#include "hw/BTThread.h"

class HWInputFaderBt : public HWInputFader, public BTI2CPolling
{
public:
    HWInputFaderBt();

    virtual bool init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void poll(BTThread* btThread);

private:
    void onRead(BTThread* thread, BTI2CPacket* packet);

    int m_slaveAddress;
    int m_channel;
    std::string m_btName;

    BTThread* m_btThread;
};

#endif // HWINPUTFADERBT_H
