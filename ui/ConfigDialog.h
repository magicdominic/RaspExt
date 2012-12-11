#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "hw/Config.h"

#include <QDialog>
#include <QAbstractTableModel>

namespace Ui {
    class ConfigDialog;
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
    void deleteInput();
    void deleteOutput();
    void deleteBt();

    void okPressed();

private:
    Ui::ConfigDialog* ui;
    ConfigInputTableModel* m_inputTableModel;
    ConfigOutputTableModel* m_outputTableModel;
    ConfigBTThreadTableModel* m_btThreadTableModel;

    Config m_config;
};



class ConfigInputTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ConfigInputTableModel(QObject *parent, Config* config);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    HWInput* get(int row) const;

private:
    Config* m_config;
};


class ConfigOutputTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ConfigOutputTableModel(QObject *parent, Config* config);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
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

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    BTThread* get(int row) const;

private:
    Config* m_config;
};

#endif // CONFIGDIALOG_H
