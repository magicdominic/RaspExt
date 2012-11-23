#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include <QDialog>

#include "script/Script.h"
#include "ui/ScriptDialogTableModel.h"

namespace Ui {
    class ScriptDialog;
}
class ConfigManager;

class ScriptDialog : public QDialog {
    Q_OBJECT
public:
    ScriptDialog(QWidget *parent, Script* script, ConfigManager *config);
    ~ScriptDialog();

protected slots:
    void addRule();
    void editRule();
    void deleteRule();

    void editVariables();

    void okPressed();

private:
    Ui::ScriptDialog* ui;
    ScriptDialogTableModel m_model;

    Script* m_script;
    ConfigManager* m_config;
};

#endif // SCRIPTDIALOG_H
