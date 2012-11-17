
#include "ui/ScriptDialogTableModel.h"


ScriptDialogTableModel::ScriptDialogTableModel(QObject* parent, Script* script) : QAbstractTableModel(parent)
{
    m_script = script;
}

int ScriptDialogTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_script->m_listRules.size();
}

int ScriptDialogTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ScriptDialogTableModel::data(const QModelIndex &index, int role) const
{
     if (!index.isValid())
         return QVariant();

     if (index.row() >= m_script->m_listRules.size() || index.row() < 0)
         return QVariant();

     if (role == Qt::DisplayRole)
     {
         Rule* rule = m_script->m_listRules.at(index.row());

         if (index.column() == 0)
             return QString::fromStdString( rule->getName() );
     }
     return QVariant();
}

QVariant ScriptDialogTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
     {
         switch (section)
         {
             case 0:
                 return tr("Name");
             default:
                 return QVariant();
         }
     }
     return QVariant();
}

bool ScriptDialogTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    m_script->m_listRules.erase( m_script->m_listRules.begin() + row);

    endRemoveRows();

    return true;
}

Rule* ScriptDialogTableModel::getRule(int row) const
{
    return m_script->m_listRules.at(row);
}

void ScriptDialogTableModel::changed()
{
    emit layoutChanged();
}

