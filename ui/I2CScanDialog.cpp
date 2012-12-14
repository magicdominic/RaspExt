
#include "ui/I2CScanDialog.h"
#include "hw/I2CThread.h"
#include "hw/BTThread.h"
#include "ui_I2CScanDialog.h"

I2CScanDialog::I2CScanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::I2CScanDialog)
{
    m_slaveAddress = -1;

    ui->setupUi(this);

    // set table models
    m_tableModel = new I2CScanTableModel(this);
    ui->tableView->setModel(m_tableModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);


    // connect all signals - slots
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

I2CScanDialog::~I2CScanDialog()
{
    delete this->ui;

    delete m_tableModel;
}

void I2CScanDialog::i2cScan(I2CThread *i2cThread)
{
    // try to read from every address
    // if it succeeds the address is valid, if it does not we assume that there is not chip on this address
    char buffer;
    for(unsigned int i = 0; i < 128; i++)
    {
        i2cThread->setSlaveAddress(i);
        if(i2cThread->read(&buffer, 1))
            m_tableModel->set(i / 16, i % 16, true);
        else
            m_tableModel->set(i / 16, i % 16, false);
    }
}

void I2CScanDialog::btI2CScan(BTThread* btThread)
{
    BTI2CPacket packets[128];

    for(unsigned int i = 0; i < 128; i++)
    {
        packets[i].request = 1;
        packets[i].error = 0;
        packets[i].slaveAddress = i;
        packets[i].read = 1;
        packets[i].commandLength = 0;
        packets[i].readLength = 1;
        packets[i].callbackFunc = std::bind(&I2CScanDialog::btI2CScanCallback, this, std::placeholders::_1, std::placeholders::_2);
    }

    btThread->sendI2CPackets(packets, 128);
}

void I2CScanDialog::btI2CScanCallback(BTThread* btThread, BTI2CPacket* packet)
{
    m_tableModel->set(packet->slaveAddress / 16, packet->slaveAddress % 16, !packet->error);
}

void I2CScanDialog::okPressed()
{
    QModelIndexList indices = ui->tableView->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        m_slaveAddress = indices.front().row() * 16 + indices.front().column();
        // Only accept if the user has choosen an address
        this->done(Accepted);
    }

    // todo: warn the user that he did not select anything
}



I2CScanTableModel::I2CScanTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    memset(m_matrix, 0, sizeof m_matrix);
}

int I2CScanTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 8;
}

int I2CScanTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 16;
}

QVariant I2CScanTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= this->rowCount() || index.row() < 0 || index.column() >= this->columnCount() || index.column() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
        return m_matrix[index.row()][index.column()] ? QString("%1").arg(index.row() * 16 + index.column(), 0, 16).toUpper() : "-";
    else if(role == Qt::BackgroundRole)
    {
        if(m_matrix[index.row()][index.column()])
            return QColor(200,255,200);
        else
            return QColor(255,200,200);
    }
    else if(role == Qt::TextAlignmentRole)
        return Qt::AlignRight + Qt::AlignVCenter;

    return QVariant();
}

QVariant I2CScanTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        if(section >= 0 && section < this->columnCount())
            return QString("%1").arg(section, 0, 16).toUpper();
        else
            return QVariant();
    }
    else if(orientation == Qt::Vertical)
    {
        if(section >= 0 && section < this->rowCount())
            return QString("%1").arg(section, 0, 16).toUpper();
        else
            return QVariant();
    }

    return QVariant();
}

void I2CScanTableModel::set(int row, int column, bool used)
{
    if(row >= this->rowCount() || row < 0 || column >= this->columnCount() || column < 0)
        return;

    m_matrix[row][column] = used;

    emit dataChanged(this->index(row, 0), this->index(row, this->columnCount()));
}
