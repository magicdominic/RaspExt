#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "hw/HWInput.h"
#include "hw/HWOutput.h"
#include "script/Variable.h"

#include <list>
#include <QFrame>

class MainWindow;
class GPIOInterruptThread;
class I2CThread;
class BTThread;
class RuleTimerThread;
class Script;
class SoundManager;

class ConfigManager
{
public:
    enum ScriptState
    {
        Inactive,
        Active,
        Paused
    };

    ConfigManager(MainWindow* win);
    ~ConfigManager();

    void init();
    void deinit();

    bool load(std::string filename);
    bool save(std::string filename);

    bool addInput(HWInput* hw);
    bool addOutput(HWOutput* hw);
    bool addVariable(Variable* var);

    void removeVariable(Variable* var);
    void clear();

    ScriptState getActiveScriptState() const { return m_scriptState;}
    void stopActiveScript();
    void pauseActiveScript();
    void continueActiveScript();

    void setActiveScript(Script* script);
    Script* getActiveScript() const { return m_activeScript;}

    std::list<HWInput*> getInputList() const;
    std::list<HWOutput*> getOutputList() const;

    HWInput* getInputByName(std::string str);
    HWOutput* getOutputByName(std::string str);
    Variable* getVariableByName(std::string str);

    GPIOInterruptThread* getGPIOThread();
    I2CThread* getI2CThread();
    RuleTimerThread* getRuleTimerThread();
    SoundManager* getSoundManager() const { return m_soundManager;}

private:
    ScriptState m_scriptState;
    Script* m_activeScript;

    MainWindow* m_mainWindow;
    std::list<HWInput*> m_listInput;
    std::list<HWOutput*> m_listOutput;
    std::list<Variable*> m_listVariable;

    std::list<BTThread*> m_listBTThread;
    GPIOInterruptThread* m_gpioThread;
    I2CThread* m_i2cThread;
    RuleTimerThread* m_ruleTimer;

    SoundManager* m_soundManager;
};

#endif // CONFIGMANAGER_H
