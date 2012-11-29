#ifndef SCRIPTCONFIGTABLEMODEL_H
#define SCRIPTCONFIGTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "script/Script.h"

/**
 * @brief The ScriptInputTableModel class
 * This is TableModel is used by MainWindow to show a cool overview over the used inputs by a given script
 */
class ScriptInputTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setScript(Script* script);

private:
    Script* m_script;

    std::list<Rule::RequiredInput> m_listInput;
};

/**
 * @brief The ScriptOutputTableModel class
 * This is TableModel is used by MainWindow to show a cool overview over the used outputs by a given script
 */
class ScriptOutputTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setScript(Script* script);

private:
    Script* m_script;

    std::list<Rule::RequiredOutput> m_listOutput;
};

/**
 * @brief The ScriptVariableTableModel class
 * This is TableModel is used by MainWindow to show a cool overview over the used variables by a given script
 */
class ScriptVariableTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setScript(Script* script);

private:
    Script* m_script;

    std::list<Rule::RequiredVariable> m_listVariable;
};

#endif // SCRIPTCONFIGTABLEMODEL_H
