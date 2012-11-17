#ifndef ACTIONOUTPUT_H
#define ACTIONOUTPUT_H

#include "script/Action.h"

#include "hw/HWOutput.h"

class ActionOutput : public Action
{
public:
    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void setHWName(std::string str) { m_HWName = str;}
    std::string getHWName() const { return m_HWName;}

    void init(ConfigManager* config);
    void deinit();

    Type getType() const { return Output;}

protected:
    std::string m_HWName;
    HWOutput* m_hw;
};

#endif // ACTIONOUTPUT_H
