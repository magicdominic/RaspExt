
#include "ui/VariableTableModel.h"

VariableTableModel::VariableTableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

VariableTableModel::~VariableTableModel()
{
    for(std::vector<Variable*>::iterator it = m_vecVariables.begin(); it != m_vecVariables.end(); it++)
    {
        delete (*it);
    }
}

int VariableTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_vecVariables.size();
}

int VariableTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant VariableTableModel::data(const QModelIndex &index, int role) const
{
     if (!index.isValid())
         return QVariant();

     if (index.row() >= m_vecVariables.size() || index.row() < 0)
         return QVariant();

     if (role == Qt::DisplayRole)
     {
         Variable* variable = m_vecVariables.at(index.row());

         if (index.column() == 0)
             return QString::fromStdString( variable->getName() );
         else if (index.column() == 1)
             return QString::number( variable->getDefaultValue() );
     }
     return QVariant();
}

QVariant VariableTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
     {
         switch (section)
         {
             case 0:
                 return tr("Name");

             case 1:
                 return tr("Default Value");

             default:
                 return QVariant();
         }
     }
     return QVariant();
}

Variable* VariableTableModel::getVariable(unsigned int row) const
{
    if(row <= m_vecVariables.size())
        return m_vecVariables.at(row);

    return NULL;
}

/**
 * @brief VariableListModel::addVariable Adds a variable to the model. The variable will be deleted by this class!
 * @param variable
 */
void VariableTableModel::addVariable(Variable* variable)
{
    beginInsertRows(QModelIndex(), this->rowCount(QModelIndex()), this->rowCount(QModelIndex()));

    m_vecVariables.push_back(variable);

    endInsertRows();
}

bool VariableTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    // delete the variable, as it is no longer needed
    delete m_vecVariables.at(row);

    m_vecVariables.erase(m_vecVariables.begin() + row);

    endRemoveRows();

    return true;
}
