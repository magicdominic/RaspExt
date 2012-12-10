#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class ConfigDialog;
}

class Script;

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    ConfigDialog(QWidget *parent, std::string name);
    ~ConfigDialog();

private:
    Ui::ConfigDialog* ui;
};

#endif // CONFIGDIALOG_H
