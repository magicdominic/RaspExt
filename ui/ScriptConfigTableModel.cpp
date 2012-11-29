
#include "ui/ScriptConfigTableModel.h"

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
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_listInput.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        Rule::RequiredInput req = *std::next(m_listInput.begin(), index.row());

        if (index.column() == 0)
            return QString::fromStdString( req.name );
        else
            return QString::fromStdString( HWInput::HWInputTypeToString(req.type) );
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

void ScriptInputTableModel::setScript(Script* script)
{
    // we have to clear the list first, as otherwise the inputs used by script will be added to the ones already in there
    m_listInput.clear();

    m_script = script;

    m_script->getRequiredList(&m_listInput, NULL, NULL);

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

void ScriptOutputTableModel::setScript(Script* script)
{
    // we have to clear the list first, as otherwise the inputs used by script will be added to the ones already in there
    m_listOutput.clear();

    m_script = script;

    m_script->getRequiredList(NULL, &m_listOutput, NULL);

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

void ScriptVariableTableModel::setScript(Script* script)
{
    // we have to clear the list first, as otherwise the inputs used by script will be added to the ones already in there
    m_listVariable.clear();

    m_script = script;

    m_script->getRequiredList(NULL, NULL, &m_listVariable);

    emit layoutChanged();
}
