#include "operationpanel.h"
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QDialog>
#include "eventrecord.h"

OperationPanel::OperationPanel(QWidget *parent) : QWidget(parent)
{
    QString style = \
            ".QWidget {\
                background-color: #FAFAFA;\
                border-radius: 10px;\
             }";
    style += "QPushButton:hover {\
                background-color: #EBEBEB;\
             }"\
            "QPushButton:pressed {\
                background-color: #F8F8FF;\
            }";
    style += "QPushButton {\
                background-color: #FAFAFA;\
                border-radius: 10px;\
                height: 35px;\
             }";

    this->setStyleSheet(style);
    QHBoxLayout *thisLayout = new QHBoxLayout();
    this->setLayout(thisLayout);

    QWidget *mianWidget = new QWidget();
    thisLayout->addWidget(mianWidget);

    QGridLayout *mainLayout = new QGridLayout();
    mianWidget->setLayout(mainLayout);

    QPushButton *configBtn = new QPushButton("配置");
    EventRecord::addEventObj(configBtn, this, "配置");
    configBtn->setIcon(QIcon(":/icon/save.png"));
    mainLayout->addWidget(configBtn, 0, 0);
    connect(configBtn, &QPushButton::clicked, this, [this] {
        emit active(activeConfig);
    });

    QPushButton *autoConfigBtn = new QPushButton("连续配置");
    EventRecord::addEventObj(autoConfigBtn, this, "连续配置");
    autoConfigBtn->setIcon(QIcon(":/icon/switch.png"));
    mainLayout->addWidget(autoConfigBtn, 0, 1);
    connect(autoConfigBtn, &QPushButton::clicked, this, [this] {
        emit active(activeAutoConfig);
    });

    QPushButton *otherBtn = new QPushButton("更多");
    otherBtn->setIcon(QIcon(":/icon/more.png"));
    mainLayout->addWidget(otherBtn, 0, 2);

    QString stateBtn = "QPushButton{background-color: transparent;}";
    QPushButton *helpBtn = new QPushButton();
    helpBtn->setMaximumWidth(30);
    helpBtn->setIcon(QIcon(":/icon/help.png"));
    mainLayout->addWidget(helpBtn, 0, 3);
    helpBtn->setStyleSheet(stateBtn);
    QObject::connect(helpBtn, &QPushButton::clicked, this, [this, helpBtn] {
        QDialog helpDialog(this);
        QHBoxLayout previewLayout;
        helpDialog.setWindowTitle("帮助文档");

        QPoint btnpos = helpBtn->pos();
        QSize btnsize = helpBtn->size();

        helpDialog.setGeometry(mapToGlobal(btnpos).x() + btnsize.width() * 2,\
                               mapToGlobal(btnpos).y() + btnsize.height() * 2, 600, 300);

        helpDialog.exec();
        helpDialog.deleteLater();
    });

    QGridLayout *otherLayout = new QGridLayout();
    QWidget *otherWidget = new QWidget();
    otherWidget->setLayout(otherLayout);
    mainLayout->addWidget(otherWidget, 1, 0, 1, 3);

    QPushButton *saveasBtn = new QPushButton("另存配置");
    EventRecord::addEventObj(saveasBtn, this, "另存配置");
    saveasBtn->setIcon(QIcon(":/icon/saveas.png"));
    otherLayout->addWidget(saveasBtn, 0, 0);
    connect(saveasBtn, &QPushButton::clicked, this, [this] {
        emit active(activeSaveas);
    });

    QPushButton *previewBtn = new QPushButton("预览");
    previewBtn->setIcon(QIcon(":/icon/preview.png"));
    otherLayout->addWidget(previewBtn, 0, 1);
    connect(previewBtn, &QPushButton::clicked, this, [this] {
        emit active(activePreview);
    });

    QPushButton *recalBtn = new QPushButton("回溯跟踪");
    recalBtn->setIcon(QIcon(":/icon/recall.png"));
    otherLayout->addWidget(recalBtn, 0, 2);
    connect(recalBtn, &QPushButton::clicked, this, [this] {
        emit active(activeRecall);
    });

    QPushButton *debugBtn = new QPushButton("调试窗口");
    debugBtn->setIcon(QIcon(":/icon/debug.png"));
    otherLayout->addWidget(debugBtn, 0, 3);
    connect(debugBtn, &QPushButton::clicked, this, [this] {
        emit active(activeDebug);
    });

    connect(otherBtn, &QPushButton::clicked, this, [otherWidget] {
        otherWidget->isHidden() ? otherWidget->show() :\
                                  otherWidget->hide();
    });
    otherWidget->hide();
}
