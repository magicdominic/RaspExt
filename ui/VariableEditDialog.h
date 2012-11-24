#ifndef VARIABLEEDITDIALOG_H
#define VARIABLEEDITDIALOG_H

#include <QDialog>

#include "script/Variable.h"

namespace Ui {
    class VariableEditDialog;
}

class Script;

class VariableEditDialog : public QDialog {
    Q_OBJECT
public:
    VariableEditDialog(QWidget *parent, Variable* variable);
    ~VariableEditDialog();

private slots:
    void okPressed();

private:
    Ui::VariableEditDialog* ui;

    Variable* m_variable;
};

#endif // VARIABLEEDITDIALOG_H
