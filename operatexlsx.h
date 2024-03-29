#ifndef OPERATEXLSX_H
#define OPERATEXLSX_H

#include <QObject>
#include <QProgressBar>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QTableView>

class OperateXlsx : public QObject
{
    Q_OBJECT
public:
    explicit OperateXlsx(QObject *parent = nullptr);
    void modelToXlsx(QStandardItemModel &model, const QString &file, QProgressBar *proBar);
    void modelToXlsx(QTableView &table, const QString &file, QProgressBar *proBar);
signals:

};

#endif // OPERATEXLSX_H
