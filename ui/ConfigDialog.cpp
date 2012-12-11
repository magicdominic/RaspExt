
#include "ui/ConfigDialog.h"
#include "hw/HWInput.h"
#include "hw/HWOutput.h"
#include "hw/BTThread.h"
#include "ui_ConfigDialog.h"

#include <QMessageBox>

ConfigDialog::ConfigDialog(QWidget *parent, std::string name) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    // only try to load config if the name is not empty
    // empty means a new Config
    if( !name.empty() )
        m_config.load(name);


    // fill in values
    ui->editName->setText( QString::fromStdString( m_config.getName() ) );


    // set table models
    m_inputTableModel = new ConfigInputTableModel(this, &m_config);
    ui->tableInputs->setModel(m_inputTableModel);
    ui->tableInputs->horizontalHeader()->setStretchLastSection(true);

    m_outputTableModel = new ConfigOutputTableModel(this, &m_config);
    ui->tableOutputs->setModel(m_outputTableModel);
    ui->tableOutputs->horizontalHeader()->setStretchLastSection(true);

    m_btThreadTableModel = new ConfigBTThreadTableModel(this, &m_config);
    ui->tableBtBoards->setModel(m_btThreadTableModel);
    ui->tableBtBoards->horizontalHeader()->setStretchLastSection(true);


    // connect all signals - slots
    connect(ui->buttonDeleteInput, SIGNAL(clicked()), this, SLOT(deleteInput()));
    connect(ui->buttonDeleteOutput, SIGNAL(clicked()), this, SLOT(deleteOutput()));
    connect(ui->buttonDeleteBt, SIGNAL(clicked()), this, SLOT(deleteBt()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

ConfigDialog::~ConfigDialog()
{
    delete this->ui;

    delete m_inputTableModel;
    delete m_outputTableModel;
    delete m_btThreadTableModel;
}

void ConfigDialog::deleteInput()
{
    QModelIndexList indices = ui->tableInputs->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        m_inputTableModel->removeRow(row);
    }
}

void ConfigDialog::deleteOutput()
{
    QModelIndexList indices = ui->tableOutputs->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        m_outputTableModel->removeRow(row);
    }
}

void ConfigDialog::deleteBt()
{
    QModelIndexList indices = ui->tableBtBoards->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        m_btThreadTableModel->removeRow(row);
    }
}

void ConfigDialog::okPressed()
{
    if( ui->editName->text().size() == 0 )
    {
        // name is empty, we cannot save a script with an empty name
        QMessageBox(QMessageBox::Warning,
                    "Empty Name",
                    "The name of a config cannot be empty! Please fill in a name",
                    QMessageBox::Ok,
                    this).exec();
        return;
    }

    m_config.save();

    this->done(Accepted);
}


ConfigInputTableModel::ConfigInputTableModel(QObject *parent, Config *config) : QAbstractTableModel(parent)
{
    m_config = config;
}

int ConfigInputTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_config->m_listInput.size();
}

int ConfigInputTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ConfigInputTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_config->m_listInput.size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        HWInput* hw = *std::next(m_config->m_listInput.begin(), index.row());

        if(index.column() == 0)
            return QString::fromStdString( hw->getName() );
        else
            return QString::fromStdString( HWInput::HWInputTypeToString( hw->getType() ) );
    }

    return QVariant();
}

QVariant ConfigInputTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "Type";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ConfigInputTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    // delete the variable, as it is no longer needed
    std::list<HWInput*>::iterator it = std::next(m_config->m_listInput.begin(), row);
    delete *it;

    m_config->m_listInput.erase(it);

    endRemoveRows();

    return true;
}


ConfigOutputTableModel::ConfigOutputTableModel(QObject *parent, Config *config) : QAbstractTableModel(parent)
{
    m_config = config;
}

int ConfigOutputTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_config->m_listOutput.size();
}

int ConfigOutputTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ConfigOutputTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_config->m_listOutput.size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        HWOutput* hw = *std::next(m_config->m_listOutput.begin(), index.row());

        if(index.column() == 0)
            return QString::fromStdString( hw->getName() );
        else
            return QString::fromStdString( HWOutput::HWOutputTypeToString( hw->getType() ) );
    }

    return QVariant();
}

QVariant ConfigOutputTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "Type";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ConfigOutputTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    // delete the variable, as it is no longer needed
    std::list<HWOutput*>::iterator it = std::next(m_config->m_listOutput.begin(), row);
    delete *it;

    m_config->m_listOutput.erase(it);

    endRemoveRows();

    return true;
}


ConfigBTThreadTableModel::ConfigBTThreadTableModel(QObject *parent, Config *config) : QAbstractTableModel(parent)
{
    m_config = config;
}

int ConfigBTThreadTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_config->m_listBTThread.size();
}

int ConfigBTThreadTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ConfigBTThreadTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_config->m_listBTThread.size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        BTThread* bt = *std::next(m_config->m_listBTThread.begin(), index.row());

        if(index.column() == 0)
            return QString::fromStdString( bt->getName() );
        else
            return QString::fromStdString( bt->getBTAddr() );
    }

    return QVariant();
}

QVariant ConfigBTThreadTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "Address";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ConfigBTThreadTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    // delete the variable, as it is no longer needed
    std::list<BTThread*>::iterator it = std::next(m_config->m_listBTThread.begin(), row);
    delete *it;

    m_config->m_listBTThread.erase(it);

    endRemoveRows();

    return true;
}
