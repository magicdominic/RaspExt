#ifndef SCRIPTDIALOGTABLEMODEL_H
#define SCRIPTDIALOGTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "script/Script.h"

class ScriptDialogTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ScriptDialogTableModel(QObject *parent, Script* script);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    Rule* getRule(int rule) const;
    void changed();

private:
    Script* m_script;
};

#endif // RULETABLEMODEL_H
