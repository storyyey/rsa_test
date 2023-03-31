#ifndef TABLESWIDGET_H
#define TABLESWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QMap>
#include <QMenu>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QProgressBar>
#include <QTabWidget>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QRegExp>
#include <QMessageBox>
#include <QFileDialog>
#include "sensordata.h"
#include "operatexlsx.h"
#include "database.h"

class TablesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TablesWidget(QWidget *parent = nullptr);
    void addItem(SensorData::configRecord &data);
    void resetTable();
private:
    QString alltype = "-- 全部 --";
    QStringList sensorTypeList;
    QStringList itemTitle;
    QStandardItemModel *tabModel;
    QTableView *tabView;
    QPushButton *exportBtn;
    QProgressBar *progress;
    QComboBox *conditionBox;
    QPushButton *screenBtn;
    QMenu *rmenu;
    int selectRow;
    int selectColumn;
signals:

private slots:
    void selectConfigRecordSlot(SensorData::configRecord &data);
    void exportSlot();
};

#endif // TABLESWIDGET_H
