#ifndef HW_CONFIG_H
#define HW_CONFIG_H

#include <list>
#include <string>

class HWInput;
class HWOutput;
class BTThread;

class Config
{
public:
    ~Config();

    bool load(std::string name);
    bool save();

    void clear();

    void setName(std::string name) { m_name = name;}
    std::string getName() const { return m_name;}

    std::list<HWInput*> m_listInput;
    std::list<HWOutput*> m_listOutput;
    std::list<BTThread*> m_listBTThread;
private:
    std::string m_name;
};

#endif // HW_CONFIG_H
