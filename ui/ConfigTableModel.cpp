
#include "ui/ConfigTableModel.h"

#include <qdir.h>

ConfigTableModel::ConfigTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    this->refresh();
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

    return "";
}

void ConfigTableModel::refresh()
{
    m_vec.clear();

    QDir directory = QDir("config/");
    QStringList entries = directory.entryList(QStringList("*.xml"));

    for(QStringList::iterator it = entries.begin(); it != entries.end(); it++)
    {
        QString name = (*it);

        // remove ".xml" ending
        name.chop(4);

        m_vec.push_back(name.toStdString());
    }

   emit layoutChanged();
}

bool ConfigTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    QString filename = "config/";
    filename.append( m_vec.at(row).c_str() );
    filename.append(".xml");

    // delete the actual file
    QFile::remove(filename);

    beginRemoveRows(QModelIndex(), row, row);

    m_vec.erase(m_vec.begin() + row);

    endRemoveRows();

    return true;
}
