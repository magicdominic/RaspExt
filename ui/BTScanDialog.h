#ifndef BTSCANDIALOG_H
#define BTSCANDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>

namespace Ui {
    class BTScanDialog;
}

class BTScanTableModel;

class BTScanDialog : public QDialog {
    Q_OBJECT
public:
    BTScanDialog(QWidget *parent);
    ~BTScanDialog();

    std::pair<std::string, std::string> getNameAddr() const;

private slots:
    void refresh();
    void okPressed();

private:
    Ui::BTScanDialog* ui;

    BTScanTableModel* m_tableModel;
};

class BTScanTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    BTScanTableModel(QObject* parent) : QAbstractTableModel(parent) {};

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void add(std::string name, std::string addr);
    std::pair<std::string, std::string> get(int row) const;

private:
    std::vector<std::pair<std::string, std::string>> m_vec;
};

#endif // BTSCANDIALOG_H
