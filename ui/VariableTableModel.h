#ifndef VARIABLELISTMODEL_H
#define VARIABLELISTMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "script/Script.h"

class VariableTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    VariableTableModel(QObject *parent=0);
    ~VariableTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Variable* getVariable(unsigned int row) const;
    void addVariable(Variable* variable);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

private:
    std::vector<Variable*> m_vecVariables;
};

#endif // VARIABLELISTMODEL_H
