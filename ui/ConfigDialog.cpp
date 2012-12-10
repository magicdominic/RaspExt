
#include "ui/ConfigDialog.h"
#include "ui_ConfigDialog.h"

#include <QMessageBox>

ConfigDialog::ConfigDialog(QWidget *parent, std::string name) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
}

ConfigDialog::~ConfigDialog()
{
    delete this->ui;
}
