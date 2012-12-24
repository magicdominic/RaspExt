#ifndef HWINPUT_H
#define HWINPUT_H

#include <string>
#include <list>

class ConfigManager;
class HWInputListener;

class QDomElement;
class QDomDocument;


/**
 * @brief The HWInput class implements all important methods to load, save, initialize, deinitialize and use an input object.
 */
class HWInput
{
public:
    enum HWInputType
    {
        Button = 0,
        Fader = 1,
        EINVALID
    };
    static std::string HWInputTypeToString(HWInputType type);
    static HWInputType StringToHWInputType(std::string str);

    enum HWType
    {
        Dummy = 0,
        I2C = 1,
        BtI2C = 2,
        Bt = 3,
    };

    HWInput();
    virtual ~HWInput();

    virtual bool init(ConfigManager* config);
    virtual void deinit(ConfigManager* config);

    static HWInput* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    virtual HWInputType getType() const = 0;
    virtual HWType getHWType() const { return Dummy;}
    void setName(std::string name) { m_name = name;}
    std::string getName() const { return m_name;}
    void setOverride(bool b);
    bool getOverride() const;

    void registerInputListener(HWInputListener* listener);
    void unregisterInputListener(HWInputListener* listener);

protected:
    virtual void inputChanged();

    bool m_bOverride;
    std::string m_name;
    std::list<HWInputListener*> m_listListeners;
};

#endif // HWINPUT_H
