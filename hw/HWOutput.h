#ifndef HWOUTPUT_H
#define HWOUTPUT_H

#include <string>
#include <list>

class HWOutputListener;
class ConfigManager;
class QDomElement;
class QDomDocument;

/**
 * @brief The HWOuptut class implements all important methods to load, save, initialize, deinitialize and use an output object.
 */
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

    enum HWType
    {
        Dummy = 0,
        I2C = 1,
        BtI2C = 2,
    };

    static std::string HWOutputTypeToString(HWOutputType type);
    static HWOutputType StringToHWOutputType(std::string str);

    HWOutput();
    virtual ~HWOutput();

    virtual void init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWOutput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    HWOutputType getType() const { return m_type;}
    virtual HWType getHWType() const { return Dummy;}
    void setName(std::string name) { m_name = name;}
    std::string getName() const { return m_name;}
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
