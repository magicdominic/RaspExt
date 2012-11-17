
#include "script/Script.h"
#include "util/Debug.h"
#include "ConfigManager.h"

#include <QDomDocument>
#include <QFile>

Script::Script()
{
    m_config = NULL;
}

Script::~Script()
{
    for(std::vector<Rule*>::iterator ruleIt = m_listRules.begin(); ruleIt != m_listRules.end(); ruleIt++)
    {
        delete (*ruleIt);
    }

    for(std::list<Variable*>::iterator varIt = m_listVars.begin(); varIt != m_listVars.end(); varIt++)
    {
        delete (*varIt);
    }
}

Script* Script::load(std::string name)
{
    std::string filename = "scripts/";
    filename.append(name);
    filename.append(".xml");

    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        pi_warn("Could not open file");
        return NULL;
    }

    QDomDocument document;
    document.setContent(&file);

    QDomElement docElem = document.documentElement();

    // check if this is a valid script file
    if(docElem.tagName().toLower().compare("script") != 0)
    {
        pi_warn("Invalid configuration file: tag \"script\" is missing");
        return NULL;
    }

    Script* script = new Script();

    script->m_name = name;

    QDomElement elem = docElem.firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("rule") == 0)
        {
            Rule* rule = Rule::load(&elem);
            if(rule != NULL)
            {
                script->addRule(rule);
            }
        }
        else if(elem.tagName().toLower().compare("variable") == 0)
        {
            Variable* variable = Variable::load(&elem);
            if(variable != NULL)
            {
                script->addVariable(variable);
            }
        }
        else if(elem.tagName().toLower().compare("description") == 0)
        {
            script->m_desc = elem.text().toStdString();
        }

        elem = elem.nextSiblingElement();
    }

    file.close();

    return script;
}

bool Script::save()
{
    std::string filename = "scripts/";
    filename.append(m_name);
    filename.append(".xml");

    QFile file(filename.c_str());
    if(!file.open(QIODevice::WriteOnly))
    {
        pi_warn("Could not open file");
        return false;
    }

    QDomDocument document;

    QDomElement script = document.createElement("script");

    document.appendChild(script);


    // save every rule
    for(std::vector<Rule*>::iterator ruleIt = m_listRules.begin(); ruleIt != m_listRules.end(); ruleIt++)
    {
        (*ruleIt)->save(&script, &document);
    }

    // save every variable
    for(std::list<Variable*>::iterator it = m_listVars.begin(); it != m_listVars.end(); it++)
    {
        (*it)->save(&script, &document);
    }

    // save description
    QDomElement desc = document.createElement("description");
    QDomText descText = document.createTextNode(QString::fromStdString(this->m_desc));
    desc.appendChild(descText);

    script.appendChild(desc);


    file.write(document.toByteArray(4));

    file.close();

    return true;
}

void Script::addRule(Rule* rule)
{
    m_listRules.push_back(rule);
}

void Script::addVariable(Variable *var)
{
    m_listVars.push_back(var);
}

void Script::init(ConfigManager *config)
{
    for(std::vector<Rule*>::iterator ruleIt = m_listRules.begin(); ruleIt != m_listRules.end(); ruleIt++)
    {
        (*ruleIt)->init(config);
    }
}

void Script::deinit()
{
    for(std::vector<Rule*>::iterator ruleIt = m_listRules.begin(); ruleIt != m_listRules.end(); ruleIt++)
    {
        (*ruleIt)->deinit();
    }
}

void Script::setConfig(ConfigManager *config)
{
    m_config = config;
}


std::list<Variable*> Script::getVariableList() const
{
    return m_listVars;
}

std::list<HWInput*> Script::getInputList() const
{
    if(m_config != NULL)
        return m_config->getInputList();
    else
    {
        pi_warn("Config not set, returning empty list");
        return std::list<HWInput*>();
    }
}

std::list<HWOutput*> Script::getOutputList() const
{
    if(m_config != NULL)
        return m_config->getOutputList();
    else
    {
        pi_warn("Config not set, returning empty list");
        return std::list<HWOutput*>();
    }
}

std::vector<Rule*> Script::getRuleList() const
{
    return m_listRules;
}

Rule* Script::getRuleByName(std::string name) const
{
    const char* cstr = name.c_str();
    for(int i = 0; i < m_listRules.size(); i++)
    {
        if(strcasecmp( m_listRules[i]->getName().c_str(), cstr) == 0)
            return m_listRules[i];
    }

    return NULL;
}
