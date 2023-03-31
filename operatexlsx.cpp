#include "operatexlsx.h"
#include "QtXlsx/xlsxdocument.h"

QTXLSX_USE_NAMESPACE

OperateXlsx::OperateXlsx(QObject *parent) : QObject(parent)
{

}

void OperateXlsx::modelToXlsx(QStandardItemModel &model, const QString &file, QProgressBar *proBar)
{
    Document xlsx;
    int row = 0, column = 0;

    if (!model.rowCount()) {
        QMessageBox::information(NULL, "导出结束", QString("无数据"),
                             QMessageBox::Yes, QMessageBox::Yes);

        return ;
    }

    xlsx.addSheet("sheet1");

    if (proBar) {
        proBar->show();
        proBar->setMinimum(0);
        proBar->setMaximum(model.rowCount());
    }
    for (column = 1; column <= model.columnCount() ; column++) {
        xlsx.setColumnWidth(column, 20);
        QStandardItem *item = model.horizontalHeaderItem(column - 1);
        if (item) {
            xlsx.write(1, column, item->text());
        }
    }

    for (row = 2; row <= model.rowCount() + 1; row++) {
        for (column = 1; column <= model.columnCount() ; column++) {
            QStandardItem *item = model.item(row - 2, column - 1);
            if (item) {
                xlsx.write(row, column, item->text());
            }
        }
        if (proBar) {
            proBar->setValue(row - 1);
        }
    }
    if (proBar) {
        proBar->setValue(model.rowCount());
    }
    xlsx.saveAs(file + ".xlsx");
    QMessageBox::information(NULL, "导出结束", QString("导出成功=>%1").arg(file + ".xlsx"),
                             QMessageBox::Yes, QMessageBox::Yes);
    if (proBar) {
        proBar->reset();
        proBar->hide();
    }
}

void OperateXlsx::modelToXlsx(QTableView &table, const QString &file, QProgressBar *proBar)
{
    QStandardItemModel *model = (QStandardItemModel *)table.model();

    Document xlsx;
    int row = 0, column = 0;
    int rowxlsx = 1;

    if (!model->rowCount()) {
        QMessageBox::information(NULL, "导出结束", QString("无数据"),
                             QMessageBox::Yes, QMessageBox::Yes);

        return ;
    }

    xlsx.addSheet("sheet1");

    if (proBar) {
        proBar->show();
        proBar->setMinimum(0);
        proBar->setMaximum(model->rowCount());
    }
    for (column = 1; column <= model->columnCount(); column++) {
        xlsx.setColumnWidth(column, 20);
        QStandardItem *item = model->horizontalHeaderItem(column - 1);
        if (item) {
            xlsx.write(rowxlsx, column, item->text());
        }
    }
    rowxlsx++;

    for (row = 0; row < model->rowCount(); row++) {
        if (!table.isRowHidden(row)) {
            for (column = 1; column <= model->columnCount(); column++) {
                QStandardItem *item = model->item(row, column - 1);
                if (item) {
                    xlsx.write(rowxlsx, column, item->text());
                }
            }
            rowxlsx++;
        }
        if (proBar) {
            proBar->setValue(row);
        }
    }
    if (proBar) {
        proBar->setValue(model->rowCount());
    }
    xlsx.saveAs(file + ".xlsx");
    QMessageBox::information(NULL, "导出结束", QString("导出成功=>%1").arg(file + ".xlsx"),
                             QMessageBox::Yes, QMessageBox::Yes);
    if (proBar) {
        proBar->reset();
        proBar->hide();
    }
}
