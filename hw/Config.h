#ifndef CONFIG_H
#define CONFIG_H

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

    void setName(std::string name) { m_name;}
    std::string getName() const { return m_name;}

private:
    std::string m_name;
    std::list<HWInput*> m_listInput;
    std::list<HWOutput*> m_listOutput;
    std::list<BTThread*> m_listBTThread;
};

#endif // CONFIG_H
