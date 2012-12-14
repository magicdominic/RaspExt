#ifndef I2CSCANDIALOG_H
#define I2CSCANDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>

namespace Ui {
    class I2CScanDialog;
}

class I2CThread;
class BTThread;
class BTI2CPacket;
class I2CScanTableModel;

class I2CScanDialog : public QDialog {
    Q_OBJECT
public:
    I2CScanDialog(QWidget *parent);
    ~I2CScanDialog();

    void i2cScan(I2CThread* i2cThread);
    void btI2CScan(BTThread* btThread);

    int getAddress() const { return m_slaveAddress;}

private slots:
    void okPressed();

private:
    void btI2CScanCallback(BTThread* btThread, BTI2CPacket* packet);

    Ui::I2CScanDialog* ui;

    I2CScanTableModel* m_tableModel;
    int m_slaveAddress;
};

class I2CScanTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    I2CScanTableModel(QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void set(int row, int column, bool used);

private:
    bool m_matrix[8][16];
};

#endif // I2CSCANDIALOG_H
