#ifndef HWINPUT_H
#define HWINPUT_H

#include <string>
#include <list>

class ConfigManager;
class HWInputListener;

class QDomElement;
class QDomDocument;


// Basis Klasse, von der alle HWInput-Klassen erben
class HWInput
{
public:
    enum HWInputType
    {
        Button = 0,
        Fader = 1,
        EINVALID,
    };
    static std::string HWInputTypeToString(HWInputType type);
    static HWInputType StringToHWInputType(std::string str);

    HWInput();
    virtual ~HWInput();

    virtual bool init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    virtual HWInputType getType() const = 0;
    void setName(std::string str);
    std::string getName() const;
    void setOverride(bool b);
    bool getOverride() const;

    void registerInputListener(HWInputListener* listener); // registriert eine Klasse fuer den Event, ruft den Event einmal direkt auf
    void unregisterInputListener(HWInputListener* listener); // deregistriert eine Klasse fuer den Event

protected:
    void inputChanged();

    bool m_bOverride;
    std::string m_name;
    std::list<HWInputListener*> m_listListeners;
};

#endif // HWINPUT_H
