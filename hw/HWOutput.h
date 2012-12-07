#ifndef HWOUTPUT_H
#define HWOUTPUT_H

#include <string>
#include <list>

class HWOutputListener;
class ConfigManager;
class QDomElement;
class QDomDocument;

// Basis Klasse, von der alle HWInput-Klassen erben
class HWOutput
{
public:
    enum HWOutputType
    {
        Relay = 0,
        DCMotor = 1,
        LED = 2,
        Stepper = 3,
        GPO = 4,
        EINVALID
    };

    static std::string HWOutputTypeToString(HWOutputType type);
    static HWOutputType StringToHWOutputType(std::string str);

    HWOutput();
    virtual ~HWOutput();

    virtual void init(ConfigManager* config);
    virtual void deinit();

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    HWOutputType getType() const { return m_type;}
    void setName(std::string str);
    std::string getName() const;
    void setOverride(bool b);
    bool getOverride() const;

    void registerOutputListener(HWOutputListener* listener);
    void unregisterOutputListener(HWOutputListener* listener);

protected:
    virtual void outputChanged();

    HWOutputType m_type;
    bool m_bOverride;
    std::string m_name;
    std::list<HWOutputListener*> m_listListeners;
};

#endif // HWOUTPUT_H
