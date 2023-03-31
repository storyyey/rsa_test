#include "tableswidget.h"

TablesWidget::TablesWidget(QWidget *parent) : QWidget(parent)
{
    QString thisStyle = \
            "QComboBox {\
                border:1px solid #63B8FF;\
                border-radius:6px;\
                height: 30px;\
             }";
    thisStyle += "QTableView {\
              border: 3px solid #FFFFFF;\
              border-radius: 6px;\
             }";

    thisStyle += ".QWidget {\
                background-color: #FFFFFF;\
             }";
    thisStyle += "QPushButton:hover {\
                background-color: #EBEBEB;\
             }"\
            "QPushButton:pressed {\
                background-color: #F8F8FF;\
            }";
    thisStyle += "QPushButton {\
                background-color: #FAFAFA;\
                border-radius: 6px;\
                height: 30px;\
             }";

    this->setStyleSheet(thisStyle);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);
    itemTitle << "时间" << "设备型号" << "设备地址" << "配置文件" << "状态";

    QHBoxLayout *toolBarLayout = new QHBoxLayout();

    exportBtn = new QPushButton("导出");
    exportBtn->setToolTip("数据表导出");
    exportBtn->setIcon(QIcon(":/icon/export.png"));
    toolBarLayout->addWidget(exportBtn);
    connect(exportBtn, SIGNAL(clicked()), this, SLOT(exportSlot()));

    QComboBox *prefixBox = new QComboBox();
    prefixBox->setMinimumWidth(120);
    prefixBox->addItems(itemTitle);

    sensorTypeList << alltype;
    for (int n = 1; n < SensorData::STMAX; n++) {
        sensorTypeList << SensorData::sensorCodeString((SensorData::sensorType)n);
    }
    conditionBox = new QComboBox();
    conditionBox->setMinimumWidth(180);
    conditionBox->addItems(sensorTypeList);
    toolBarLayout->addWidget(conditionBox);

    screenBtn = new QPushButton("筛选");
    screenBtn->setIcon(QIcon(":/icon/screen.png"));
    toolBarLayout->addWidget(screenBtn);
    QObject::connect(screenBtn, &QPushButton::clicked, this, [this] {
        int screencolumn = -1;

        for (int n = 0; n < itemTitle.count(); n++) {
            if (itemTitle.at(n) == "设备型号") {
                screencolumn = n;
                break;
            }
        }
        if (screencolumn < 0) {
            return ;
        }

        for (int row = 0; row < tabModel->rowCount(); row++) {
            QStandardItem *item =  tabModel->item(row, screencolumn);
            if (item) {
                if (conditionBox->currentText() == alltype ||\
                    item->text() == conditionBox->currentText()) {
                    tabView->showRow(row);
                }
                else {
                    tabView->hideRow(row);
                }
            }
        }
    });

    progress = new QProgressBar();
    toolBarLayout->addWidget(progress);
    progress->hide();
    mainLayout->addLayout(toolBarLayout);
    toolBarLayout->addStretch();
    tabModel = new QStandardItemModel();
    tabModel->setColumnCount(itemTitle.length());
    tabModel->setHorizontalHeaderLabels(itemTitle);
    tabView = new QTableView();
    tabView->setModel(tabModel);
    tabView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabView->setAlternatingRowColors(true);
    tabView->setContextMenuPolicy(Qt::CustomContextMenu);
    tabView->setSortingEnabled(true);

    mainLayout->addWidget(tabView);
}

void TablesWidget::addItem(SensorData::configRecord &data)
{
    QList<QStandardItem *> idItem = tabModel->findItems(data.addr, Qt::MatchFixedString, 2);

    if (idItem.size() == 0) {
        QList<QStandardItem *> itemList;

        QStandardItem *timeItem = new QStandardItem(data.time);
        timeItem->setToolTip(data.time);
        itemList.append(timeItem);
        itemList.append(new QStandardItem(SensorData::sensorCodeString(data.type)));
        itemList.append(new QStandardItem(data.addr));
        QStandardItem *cfileItem = new QStandardItem(data.cfile);
        cfileItem->setToolTip(data.cfile);
        itemList.append(cfileItem);
        itemList.append(new QStandardItem(data.state ? "成功":"失败"));
        tabModel->insertRow(0, itemList);
        if (conditionBox->currentText() != alltype &&\
            conditionBox->currentText() != SensorData::sensorCodeString(data.type)) {
            tabView->hideRow(0);
        }
    } else {
        int row = 0;
        foreach(QStandardItem * entry, idItem) {
            row = entry->row();
            tabModel->item(row, 0)->setText(data.time);
            tabModel->item(row, 1)->setText(SensorData::sensorCodeString(data.type));
            tabModel->item(row, 2)->setText(data.addr);
            tabModel->item(row, 3)->setText(data.cfile);
            tabModel->item(row, 4)->setText(data.state ? "成功":"失败");
        }
    }
}

void TablesWidget::resetTable()
{
    tabModel->clear();
    tabModel->setHorizontalHeaderLabels(itemTitle);
}

void TablesWidget::selectConfigRecordSlot(SensorData::configRecord &data)
{
    addItem(data);
}

void TablesWidget::exportSlot()
{
    OperateXlsx oxlsx;
    QString fileName;

    screenBtn->click();

    if (conditionBox->currentText() != alltype) {
        fileName = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_") + \
                conditionBox->currentText();
    }
    else {
        fileName = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_") + "传感器配置记录";
    }
    oxlsx.modelToXlsx(*tabView, fileName, progress);
}
