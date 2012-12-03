
#include "ui/ScriptConfigTableModel.h"
#include "ConfigManager.h"

int ScriptInputTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_listInput.size();
}

int ScriptInputTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ScriptInputTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_listInput.size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        Rule::RequiredInput req = *std::next(m_listInput.begin(), index.row());

        if(index.column() == 0)
            return QString::fromStdString( req.name );
        else
            return QString::fromStdString( HWInput::HWInputTypeToString(req.type) );
    }
    else if(role == Qt::BackgroundRole)
    {
        Rule::RequiredInput req = *std::next(m_listInput.begin(), index.row());

        if(req.exists)
            return QColor(200,255,200);
        else
            return QColor(255,200,200);
    }

    return QVariant();
}

QVariant ScriptInputTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
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

void ScriptInputTableModel::set(std::list<Rule::RequiredInput> &listInput)
{
    m_listInput = listInput;

    emit layoutChanged();
}



int ScriptOutputTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_listOutput.size();
}

int ScriptOutputTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ScriptOutputTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_listOutput.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        Rule::RequiredOutput req = *std::next(m_listOutput.begin(), index.row());

        if (index.column() == 0)
            return QString::fromStdString( req.name );
        else
            return QString::fromStdString( HWOutput::HWOutputTypeToString(req.type) );
    }
    else if(role == Qt::BackgroundRole)
    {
        Rule::RequiredOutput req = *std::next(m_listOutput.begin(), index.row());

        if(req.exists)
            return QColor(200,255,200);
        else
            return QColor(255,200,200);
    }

    return QVariant();
}

QVariant ScriptOutputTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
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

void ScriptOutputTableModel::set(std::list<Rule::RequiredOutput> &listOutput)
{
    m_listOutput = listOutput;

    emit layoutChanged();
}




int ScriptVariableTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_listVariable.size();
}

int ScriptVariableTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ScriptVariableTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_listVariable.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        Rule::RequiredVariable req = *std::next(m_listVariable.begin(), index.row());

        if (index.column() == 0)
            return QString::fromStdString( req.name );
    }
    else if(role == Qt::BackgroundRole)
    {
        Rule::RequiredVariable req = *std::next(m_listVariable.begin(), index.row());

        if(req.exists)
            return QColor(200,255,200);
        else
            return QColor(255,200,200);
    }

    return QVariant();
}

QVariant ScriptVariableTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

void ScriptVariableTableModel::set(std::list<Rule::RequiredVariable> &listVariable)
{
    m_listVariable = listVariable;

    emit layoutChanged();
}
