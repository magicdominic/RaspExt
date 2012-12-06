#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include <QDialog>

#include "script/Rule.h"
#include "ui/ActionTableModel.h"
#include "ui/ConditionTableModel.h"

namespace Ui {
    class RuleDialog;
}

class Script;

class RuleDialog : public QDialog {
    Q_OBJECT
public:
    RuleDialog(QWidget *parent, Rule* rule, Script* script);
    ~RuleDialog();

private slots:
    void addCondition();
    void editCondition();
    void deleteCondition();

    void addAction();
    void editAction();
    void deleteAction();
    void actionUp();
    void actionDown();

    void closePressed();

private:
    Ui::RuleDialog* ui;
    ActionTableModel m_actionModel;
    ConditionTableModel m_conditionModel;

    Rule* m_rule;
    Script* m_script;
};

#endif // RULEDIALOG_H
