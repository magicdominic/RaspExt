#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "hw/HWInput.h"
#include "hw/HWOutput.h"
#include "script/Variable.h"
#include "ConfigManager.h"

#include "ui/InputFrame.h"
#include "ui/OutputFrame.h"
#include "ui/VariableFrame.h"
#include "ui/ScriptsTableModel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addInput(HWInput* hw);
    void addOutput(HWOutput* hw);
    void addVariable(Variable* var);
    void removeInput(HWInput* hw);
    void removeOutput(HWOutput* hw);
    void removeVariable(Variable* var);

protected slots:
    void createScript();
    void editScript();
    void selectScript();
    void deleteScript();

    void stopScript();
    void startPauseScript();

private:
    void updateScriptState();
    bool checkScript(Script* script);

    Ui::MainWindow *ui;
    ScriptsTableModel* m_scriptsModel;

    ConfigManager m_config;

    std::list<InputFrame*> m_listInputFrame;
    std::list<OutputFrame*> m_listOutputFrame;
    std::list<VariableFrame*> m_listVarFrame;
};

#endif // MAINWINDOW_H
