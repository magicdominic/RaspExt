#ifndef VARIABLELISTDIALOG_H
#define VARIABLELISTDIALOG_H

#include <QDialog>

#include "script/Variable.h"
#include "ui/VariableTableModel.h"

namespace Ui {
    class VariableListDialog;
}

class Script;

class VariableListDialog : public QDialog {
    Q_OBJECT
public:
    VariableListDialog(QWidget *parent, Script* script);
    ~VariableListDialog();

private slots:
    void okPressed();
    void addVariable();
    void editVariable();
    void deleteVariable();

private:
    Ui::VariableListDialog* ui;

    VariableTableModel m_model;
    Script* m_script;
};

#endif // VARIABLELISTDIALOG_H
