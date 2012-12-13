
#include "ui/I2CScanDialog.h"
#include "hw/I2CThread.h"
#include "ui_I2CScanDialog.h"

I2CScanDialog::I2CScanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::I2CScanDialog)
{
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
    for(unsigned int i = 0; i < 127; i++)
    {
        i2cThread->setSlaveAddress(i);
        if(i2cThread->read(&buffer, 1))
            m_tableModel->set(i / 16, i % 16, "Y");
        else
            m_tableModel->set(i / 16, i % 16, "-");
    }
}

void I2CScanDialog::okPressed()
{
    QModelIndexList indices = ui->tableView->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        // Only accept if the user has choosen an address
        this->done(Accepted);
    }
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
        return m_matrix[index.row()][index.column()];

    return QVariant();
}

QVariant I2CScanTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        if(section >= 0 && section < this->columnCount())
            return QString("%1").arg(section);
        else
            return QVariant();
    }
    else if(orientation == Qt::Vertical)
    {
        if(section >= 0 && section < this->rowCount())
            return QString("%1").arg(section);
        else
            return QVariant();
    }

    return QVariant();
}

void I2CScanTableModel::set(int row, int column, std::string data)
{
    if(row >= this->rowCount() || row < 0 || column >= this->columnCount() || column < 0)
        return;

    m_matrix[row][column] = QString::fromStdString(data);
}
