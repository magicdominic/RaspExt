
#include "ConfigManager.h"
#include "hw/BTThread.h"
#include "hw/GPIOInterruptThread.h"
#include "hw/I2CThread.h"
#include "script/RuleTimerThread.h"
#include "ui/MainWindow.h"
#include "util/Debug.h"

#include <QDomDocument>
#include <QFile>

ConfigManager::ConfigManager(MainWindow *win)
{
    m_scriptState = Inactive;
    m_activeScript = NULL;

    m_mainWindow = win;

    m_gpioThread = NULL;
    m_i2cThread = NULL;
    m_ruleTimer = new RuleTimerThread();
}

ConfigManager::~ConfigManager()
{
    if(m_gpioThread != NULL)
    {
        m_gpioThread->kill();
        delete m_gpioThread;
    }

    if(m_i2cThread != NULL)
    {
        m_i2cThread->kill();
        delete m_i2cThread;
    }

    if(m_ruleTimer != NULL)
    {
        m_ruleTimer->kill();
        delete m_ruleTimer;
    }

    // clear after killing each thread, as otherwise there could be pending operations in their queues
    this->clear();
}

void ConfigManager::init()
{
    for(std::list<HWInput*>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        (*it)->init(this);
    }

    for(std::list<HWOutput*>::iterator it = m_listOutput.begin(); it != m_listOutput.end(); it++)
    {
        (*it)->init(this);
    }
}

void ConfigManager::deinit()
{
    if(m_activeScript != NULL)
    {
        this->stopActiveScript();
    }

    for(std::list<HWInput*>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        (*it)->deinit(this);
    }

    for(std::list<HWOutput*>::iterator it = m_listOutput.begin(); it != m_listOutput.end(); it++)
    {
        (*it)->deinit();
    }
}

void ConfigManager::clear()
{
    for(std::list<HWInput*>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        // delete Pointer, we must clean the list afterwards immediatly!
        if(m_mainWindow != NULL)
            m_mainWindow->removeInput(*it);
        delete (*it);
    }
    m_listInput.clear();

    for(std::list<HWOutput*>::iterator it = m_listOutput.begin(); it != m_listOutput.end(); it++)
    {
        // delete Pointer, we must clean the list afterwards immediatly!
        if(m_mainWindow != NULL)
            m_mainWindow->removeOutput(*it);
        delete (*it);
    }
    m_listOutput.clear();

    // TODO: is this really a good idea?!?
    for(std::list<Variable*>::iterator it = m_listVariable.begin(); it != m_listVariable.end(); it++)
    {
        if(m_mainWindow != NULL)
            m_mainWindow->removeVariable(*it);
    }
    m_listVariable.clear();
}

bool ConfigManager::load(std::string filename)
{
    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        pi_warn("Could not open file");
        return false;
    }

    QDomDocument document;
    document.setContent(&file);

    QDomElement docElem = document.documentElement();

    // check if this is a valid configuration file
    if(docElem.tagName().toLower().compare("config") != 0)
    {
        pi_warn("Invalid configuration file: tag \"config\" is missing");
        return false;
    }


    QDomElement elem = docElem.firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("input") == 0)
        {
            HWInput* hw = HWInput::load(&elem);
            if(hw != NULL)
            {
                // if addInput returns false, the input is not accepted, so we better delete it
                if(!this->addInput(hw))
                {
                    delete hw;
                }
            }
        }
        else if(elem.tagName().toLower().compare("output") == 0)
        {
            HWOutput* hw = HWOutput::load(&elem);
            if(hw != NULL)
            {
                // if addOutput returns false, the output is not accepted, so we better delete it
                if(!this->addOutput(hw))
                {
                    delete hw;
                }
            }
        }
        else if(elem.tagName().toLower().compare("bluetooth") == 0)
        {
            BTThread* bt = BTThread::load(&elem);
            if(bt != NULL)
            {
                m_listBTThread.push_back(bt);
            }
        }

        elem = elem.nextSiblingElement();
    }

    file.close();

    return true;
}

bool ConfigManager::save(std::string filename)
{
    QFile file(filename.c_str());
    if(!file.open(QIODevice::WriteOnly))
    {
        pi_warn("Could not open file");
        return false;
    }

    QDomDocument document;

    QDomElement config = document.createElement("config");

    document.appendChild(config);


    for(std::list<HWInput*>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        (*it)->save(&config, &document);
    }

    for(std::list<HWOutput*>::iterator it = m_listOutput.begin(); it != m_listOutput.end(); it++)
    {
        (*it)->save(&config, &document);
    }

    // save BT Parameters
    for(std::list<BTThread*>::iterator it = m_listBTThread.begin(); it != m_listBTThread.end(); it++)
    {
        (*it)->save(&config, &document);
    }


    file.write(document.toByteArray(4));

    file.close();

    return true;
}

bool ConfigManager::addInput(HWInput *hw)
{
    // check if already in list, if yes then return false
    for(std::list<HWInput*>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        if(strcasecmp((*it)->getName().c_str(), hw->getName().c_str()) == 0)
            return false;
    }

    m_listInput.push_back(hw);

    if(m_mainWindow != NULL)
        m_mainWindow->addInput(hw);

    return true;
}

bool ConfigManager::addOutput(HWOutput *hw)
{
    for(std::list<HWOutput*>::iterator it = m_listOutput.begin(); it != m_listOutput.end(); it++)
    {
        if(strcasecmp((*it)->getName().c_str(), hw->getName().c_str()) == 0)
            return false;
    }

    m_listOutput.push_back(hw);

    if(m_mainWindow != NULL)
        m_mainWindow->addOutput(hw);

    return true;
}

bool ConfigManager::addVariable(Variable *var)
{
    for(std::list<Variable*>::iterator it = m_listVariable.begin(); it != m_listVariable.end(); it++)
    {
        if(strcasecmp((*it)->getName().c_str(), var->getName().c_str()) == 0)
            return false;
    }

    m_listVariable.push_back(var);

    if(m_mainWindow != NULL)
        m_mainWindow->addVariable(var);

    return true;
}

void ConfigManager::removeVariable(Variable *var)
{
    if(m_mainWindow != NULL)
        m_mainWindow->removeVariable(var);

    m_listVariable.remove(var);
}

void ConfigManager::setActiveScript(Script *script)
{
    // first delete old script, if any
    this->stopActiveScript();

    // start timer
    // this must be done FIRST
    m_ruleTimer->start();

    // set new script and initialize it
    m_activeScript = script;

    // add variables to config
    std::list<Variable*> listVar = m_activeScript->getVariableList();
    for(std::list<Variable*>::iterator it = listVar.begin(); it != listVar.end(); it++)
    {
        // reset variable value to defaults, because they may be altered if the script was started before
        (*it)->setToDefault();
        this->addVariable(*it);
    }

    // Initialize script, it will then acquire all resources it needs
    m_activeScript->init(this);

    m_scriptState = Active;
}

void ConfigManager::stopActiveScript()
{
    if(m_activeScript != NULL)
    {
        if(m_scriptState == Active)
        {
            // clear queue of timer
            // this must be done FIRST! Otherwise the process will crash
            m_ruleTimer->kill();
            m_ruleTimer->clear();

            // deinitialize script, thus releasing all acquired resources
            m_activeScript->deinit();

            // remove variables from config as they are no longer needed
            std::list<Variable*> listVar = m_activeScript->getVariableList();
            for(std::list<Variable*>::iterator it = listVar.begin(); it != listVar.end(); it++)
            {
                this->removeVariable(*it);
            }
        }

        m_activeScript = NULL;
        m_scriptState = Inactive;
    }
}

void ConfigManager::pauseActiveScript()
{
    if(m_activeScript != NULL && m_scriptState == Active)
    {
        m_scriptState = Paused;

        // stop timer
        // this must be done FIRST! Otherwise the process will crash
        m_ruleTimer->pauseTimer();

        // deinitialize script, this will effectively stop events from being handled
        m_activeScript->deinit();
    }
}

void ConfigManager::continueActiveScript()
{
    if(m_activeScript != NULL && m_scriptState == Paused)
    {
        m_scriptState = Active;
        // initialize script, this will effectively start the handling of events
        m_activeScript->init(this);

        // start timer
        // this must be done as last
        m_ruleTimer->continueTimer();
    }
}

std::list<HWInput*> ConfigManager::getInputList() const
{
    return m_listInput;
}

std::list<HWOutput*> ConfigManager::getOutputList() const
{
    return m_listOutput;
}

HWInput* ConfigManager::getInputByName(std::string str)
{
    for(std::list<HWInput*>::iterator it = m_listInput.begin(); it != m_listInput.end(); it++)
    {
        if(strcasecmp((*it)->getName().c_str(), str.c_str()) == 0)
            return *it;
    }

    return NULL;
}

HWOutput* ConfigManager::getOutputByName(std::string str)
{
    for(std::list<HWOutput*>::iterator it = m_listOutput.begin(); it != m_listOutput.end(); it++)
    {
        if(strcasecmp((*it)->getName().c_str(), str.c_str()) == 0)
            return *it;
    }

    return NULL;
}

Variable* ConfigManager::getVariableByName(std::string str)
{
    for(std::list<Variable*>::iterator it = m_listVariable.begin(); it != m_listVariable.end(); it++)
    {
        if(strcasecmp((*it)->getName().c_str(), str.c_str()) == 0)
            return *it;
    }

    return NULL;
}

GPIOInterruptThread* ConfigManager::getGPIOThread()
{
    if(m_gpioThread == NULL)
        m_gpioThread = new GPIOInterruptThread();

    return m_gpioThread;
}

I2CThread* ConfigManager::getI2CThread()
{
    if(m_i2cThread == NULL)
        m_i2cThread = new I2CThread();

    return m_i2cThread;
}

RuleTimerThread* ConfigManager::getRuleTimerThread()
{
    return m_ruleTimer;
}
