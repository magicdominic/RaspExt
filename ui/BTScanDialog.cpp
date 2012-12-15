
#include "ui/BTScanDialog.h"

#include "ui_BTScanDialog.h"

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

BTScanDialog::BTScanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BTScanDialog)
{
    ui->setupUi(this);

    // set table models
    m_tableModel = new BTScanTableModel(this);
    ui->tableView->setModel(m_tableModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);


    // connect all signals - slots
    connect(ui->buttonRefresh, SIGNAL(clicked()), this, SLOT(refresh()));

    connect(ui->buttonOkay, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));

    this->refresh();
}

BTScanDialog::~BTScanDialog()
{
    delete this->ui;

    delete m_tableModel;
}

void BTScanDialog::refresh()
{
    // this should be done in a seperate thread to allow gui tasks to continue
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if(dev_id < 0 || sock < 0) {
        perror("Error opening socket");
        return;
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    inquiry_info *ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 )
        perror("Error on hci_inquiry");

    for(i = 0; i < num_rsp; i++)
    {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));

        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");

        m_tableModel->add(name, addr);
    }

    free( ii );
    ::close( sock );
}

void BTScanDialog::okPressed()
{
    QModelIndexList indices = ui->tableView->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        // Only accept if the user has choosen a device
        this->done(Accepted);
    }

    // todo: warn the user that he did not select anything
}

std::pair<std::string, std::string> BTScanDialog::getNameAddr() const
{
    QModelIndexList indices = ui->tableView->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        return m_tableModel->get(indices.front().row());
    }

    return std::pair<std::string, std::string>();
}


int BTScanTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_vec.size();
}

int BTScanTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}

QVariant BTScanTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= this->rowCount() || index.row() < 0 || index.column() >= this->columnCount() || index.column() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            return QString::fromStdString( m_vec.at(index.row()).first );
        case 1:
            return QString::fromStdString( m_vec.at(index.row()).second );
        }
    }

    return QVariant();
}

QVariant BTScanTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return "Name";
        case 1:
            return "Address";
        }
    }

    return QVariant();
}

void BTScanTableModel::add(std::string name, std::string addr)
{
    beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());

    std::pair<std::string, std::string> pair(name, addr);
    m_vec.push_back(pair);

    endInsertRows();
}

std::pair<std::string, std::string> BTScanTableModel::get(int row) const
{
    return m_vec.at(row);
}
