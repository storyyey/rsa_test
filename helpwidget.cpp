#include "helpwidget.h"
#include <QUrl>
#include <QDesktopServices>
#include <QListWidget>
#include <QLayout>
#include <QDebug>

HelpWidget::HelpWidget(QWidget *parent) : QWidget(parent)
{
    QScrollArea *scrollbar = new QScrollArea();
    scrollbar->setGeometry(0, 0, 300, 200);
    scrollbar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //scrollbar->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);
    mainLayout->addWidget(scrollbar);
    QWidget *stepWidget = new QWidget();
    stepWidget->setGeometry(0, 0, 1000, 6000);
    scrollbar->setWidget(stepWidget);
    QVBoxLayout *stepLayout = new QVBoxLayout();
    stepWidget->setLayout(stepLayout);

    QLabel *funcExplain = new QLabel("页面功能区域");
    stepLayout->addWidget(funcExplain);
    QLabel *funcLabel = new QLabel();
    funcLabel->setPixmap(QPixmap(":/helpimage/func.PNG").scaled(QSize(1000, 2000), Qt::KeepAspectRatio));
    stepLayout->addWidget(funcLabel);

    QLabel *step1Explain = new QLabel("1.首先分别打开连接传感器和扫码枪设备的串口。");
    stepLayout->addWidget(step1Explain);
    QLabel *step1Label = new QLabel();
    step1Label->setPixmap(QPixmap(":/helpimage/step1.PNG"));
    stepLayout->addWidget(step1Label);

    QLabel *step2Explain = new QLabel("2.选择需要配置的传感器类型。");
    stepLayout->addWidget(step2Explain);
    QLabel *step2label = new QLabel();
    step2label->setPixmap(QPixmap(":/helpimage/step2.PNG"));
    stepLayout->addWidget(step2label);
    stepLayout->addStretch();

    stepLayout->addStretch();
}

HelpWidget::~HelpWidget()
{
    qDebug() << "~HelpWidget()";
}

void HelpWidget::help()
{
   // QDesktopServices::openUrl(QUrl(QLatin1String("www.baidu.com")));
}
