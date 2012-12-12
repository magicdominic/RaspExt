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
}

class ConfigInputTableModel;
class ConfigOutputTableModel;
class ConfigBTThreadTableModel;

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    ConfigDialog(QWidget *parent, std::string name);
    ~ConfigDialog();

private slots:
    void addInput();
    void editInput();
    void deleteInput();

    void deleteOutput();

    void addBt();
    void editBt();
    void deleteBt();

    void okPressed();

private:
    Ui::ConfigDialog* ui;
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
    void modifyRow(int row, HWInput *btThread);

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

private:
    Ui::ConfigBTDialog* ui;
};

class IConfigInputWidget;

class ConfigInputDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigInputDialog(QWidget *parent);
    ~ConfigInputDialog();

    void edit(HWInput* btThread);

    HWInput* assemble() const;

private slots:
    void comboChanged(int index);

private:
    Ui::ConfigInputDialog* ui;

    IConfigInputWidget* m_baseWidget;
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
    ConfigInputButtonWidget(QWidget* parent);

    HWInput* assemble();
    void edit(HWInput* hw);

private slots:
    void typeChanged(int index);

private:
    QComboBox* m_comboType;

    QLabel* m_labelBtBoard;
    QComboBox* m_comboBtBoard;

    QLabel* m_labelI2CAddr;
    QSpinBox* m_spinI2CAddr;

    QLabel* m_labelPort;
    QSpinBox* m_spinPort;
};

#endif // CONFIGDIALOG_H
