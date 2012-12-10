#ifndef CONFIGTABLEMODEL_H
#define CONFIGTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <string>

class ConfigTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ConfigTableModel(QObject *parent=0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    std::string get(unsigned int row) const;
private:
    std::vector<std::string> m_vec;
};

#endif // CONFIGTABLEMODEL_H
