
#include "ui/ConfigTableModel.h"

#include <qdir.h>

ConfigTableModel::ConfigTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    QDir directory = QDir("config/");
    QStringList entries = directory.entryList(QStringList("*.xml"));

    for(QStringList::iterator it = entries.begin(); it != entries.end(); it++)
    {
        QString name = (*it);

        // remove ".xml" ending
        name.chop(4);

        m_vec.push_back(name.toStdString());
    }
}

int ConfigTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_vec.size();
}

int ConfigTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ConfigTableModel::data(const QModelIndex &index, int role) const
{
     if (!index.isValid())
         return QVariant();

     if (index.row() >= this->m_vec.size() || index.row() < 0)
         return QVariant();

     if (role == Qt::DisplayRole)
     {
         std::string name = m_vec.at(index.row());

         if (index.column() == 0)
             return QString::fromStdString( name );
     }
     return QVariant();
}

QVariant ConfigTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

std::string ConfigTableModel::get(unsigned int row) const
{
    if(row < m_vec.size())
        return m_vec.at(row);

    return NULL;
}
