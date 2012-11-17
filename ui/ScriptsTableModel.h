#ifndef SCRIPTSTABLEMODEL_H
#define SCRIPTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "script/Script.h"

class ScriptsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ScriptsTableModel(QObject *parent=0);
    ~ScriptsTableModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Script* getScript(unsigned int row) const;
    void addScript(Script* script);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    void changed();
private:
    std::vector<Script*> m_vecScripts;
};

#endif // SCRIPTSTABLEMODEL_H
