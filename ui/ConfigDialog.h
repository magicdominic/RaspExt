#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "hw/Config.h"

#include <QDialog>
#include <QAbstractTableModel>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>

namespace Ui {
    class ConfigDialog;
    class ConfigBTDialog;
    class ConfigInputDialog;
    class ConfigOutputDialog;
}

class ConfigManager;
class ConfigInputTableModel;
class ConfigOutputTableModel;
class ConfigBTThreadTableModel;

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    ConfigDialog(QWidget *parent, std::string name, ConfigManager* config);
    ~ConfigDialog();

    int i2cScan();
    int btI2CScan(std::string name);

    const std::list<BTThread*>* getListBTThread() const { return &m_config.m_listBTThread;}
private slots:
    void addInput();
    void editInput();
    void deleteInput();

    void addOutput();
    void editOutput();
    void deleteOutput();

    void addBt();
    void editBt();
    void deleteBt();

    void okPressed();

private:
    Ui::ConfigDialog* ui;

    ConfigManager* m_configManager;

    ConfigInputTableModel* m_inputTableModel;
    ConfigOutputTableModel* m_outputTableModel;
    ConfigBTThreadTableModel* m_btThreadTableModel;

    Config m_config;
};


// table models

class ConfigInputTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ConfigInputTableModel(QObject *parent, Config* config);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    HWInput* get(int row) const;
    void add(HWInput* btThread);
    void modifyRow(int row, HWInput *hw);

private:
    Config* m_config;
};


class ConfigOutputTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ConfigOutputTableModel(QObject *parent, Config* config);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    HWOutput* get(int row) const;
    void add(HWOutput* btThread);
    void modifyRow(int row, HWOutput *hw);

private:
    Config* m_config;
};

class ConfigBTThreadTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ConfigBTThreadTableModel(QObject *parent, Config* config);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    BTThread* get(int row) const;
    void add(BTThread* btThread);
    void modifyRow(int row, BTThread *btThread);

private:
    Config* m_config;
};


// helper dialogs

class ConfigBTDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigBTDialog(QWidget *parent);
    ~ConfigBTDialog();

    void edit(BTThread* btThread);

    BTThread* assemble() const;

private slots:
    void btScan();

private:
    Ui::ConfigBTDialog* ui;
};

class IConfigInputWidget;

class ConfigInputDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigInputDialog(ConfigDialog *parent);
    ~ConfigInputDialog();

    void edit(HWInput* hw);

    HWInput* assemble() const;

private slots:
    void comboChanged(int index);

private:
    Ui::ConfigInputDialog* ui;

    IConfigInputWidget* m_baseWidget;
};

class IConfigOutputWidget;

class ConfigOutputDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigOutputDialog(ConfigDialog *parent);
    ~ConfigOutputDialog();

    void edit(HWOutput* hw);

    HWOutput* assemble() const;

private slots:
    void comboChanged(int index);

private:
    Ui::ConfigOutputDialog* ui;

    IConfigOutputWidget* m_baseWidget;
};




class IConfigInputWidget : public QWidget
{
public:
    IConfigInputWidget(QWidget* parent) : QWidget(parent) {};
    virtual HWInput* assemble() = 0;
    virtual void edit(HWInput* hw) = 0;
};

class ConfigInputButtonWidget : public IConfigInputWidget
{
    Q_OBJECT
public:
    ConfigInputButtonWidget(QWidget* parent, ConfigDialog* configDialog);

    HWInput* assemble();
    void edit(HWInput* hw);

private slots:
    void typeChanged(int index);
    void i2cScan();

private:
    ConfigDialog* m_configDialog;

    QComboBox* m_comboType;

    QLabel* m_labelBtBoard;
    QComboBox* m_comboBtBoard;

    QLabel* m_labelI2CAddr;
    QSpinBox* m_spinI2CAddr;

    QLabel* m_labelPort;
    QSpinBox* m_spinPort;

    QPushButton* m_buttonI2CScan;
};


class ConfigInputFaderWidget : public IConfigInputWidget
{
    Q_OBJECT
public:
    ConfigInputFaderWidget(QWidget* parent, ConfigDialog* configDialog);

    HWInput* assemble();
    void edit(HWInput* hw);

private slots:
    void typeChanged(int index);
    void i2cScan();

private:
    ConfigDialog* m_configDialog;

    QComboBox* m_comboType;

    QLabel* m_labelBtBoard;
    QComboBox* m_comboBtBoard;

    QLabel* m_labelI2CAddr;
    QSpinBox* m_spinI2CAddr;

    QLabel* m_labelChannel;
    QSpinBox* m_spinChannel;

    QPushButton* m_buttonI2CScan;
};


class IConfigOutputWidget : public QWidget
{
public:
    IConfigOutputWidget(QWidget* parent) : QWidget(parent) {};
    virtual HWOutput* assemble() = 0;
    virtual void edit(HWOutput* hw) = 0;
};

class ConfigOutputDCMotorWidget : public IConfigOutputWidget
{
    Q_OBJECT
public:
    ConfigOutputDCMotorWidget(QWidget* parent, ConfigDialog* configDialog);

    HWOutput* assemble();
    void edit(HWOutput* hw);

private slots:
    void typeChanged(int index);
    void i2cScan();

private:
    ConfigDialog* m_configDialog;

    QComboBox* m_comboType;

    QLabel* m_labelBtBoard;
    QComboBox* m_comboBtBoard;

    QLabel* m_labelI2CAddr;
    QSpinBox* m_spinI2CAddr;

    QPushButton* m_buttonI2CScan;
};

class ConfigOutputStepperWidget : public IConfigOutputWidget
{
    Q_OBJECT
public:
    ConfigOutputStepperWidget(QWidget* parent, ConfigDialog* configDialog);

    HWOutput* assemble();
    void edit(HWOutput* hw);

private slots:
    void typeChanged(int index);
    void i2cScan();

private:
    ConfigDialog* m_configDialog;

    QComboBox* m_comboType;

    QLabel* m_labelBtBoard;
    QComboBox* m_comboBtBoard;

    QLabel* m_labelI2CAddr;
    QSpinBox* m_spinI2CAddr;

    QPushButton* m_buttonI2CScan;
};

#endif // CONFIGDIALOG_H
