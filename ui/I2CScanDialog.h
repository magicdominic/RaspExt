#ifndef I2CSCANDIALOG_H
#define I2CSCANDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>

namespace Ui {
    class I2CScanDialog;
}

class I2CThread;
class I2CScanTableModel;

class I2CScanDialog : public QDialog {
    Q_OBJECT
public:
    I2CScanDialog(QWidget *parent);
    ~I2CScanDialog();

    void i2cScan(I2CThread* i2cThread);

private slots:
    void okPressed();

private:
    Ui::I2CScanDialog* ui;

    I2CScanTableModel* m_tableModel;
};

class I2CScanTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    I2CScanTableModel(QObject *parent) : QAbstractTableModel(parent) {};

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void set(int row, int column, std::string data);

private:
    QString m_matrix[8][16];
};

#endif // I2CSCANDIALOG_H
