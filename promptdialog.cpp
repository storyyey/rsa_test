#include "promptdialog.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QMovie>
#include <QDateTime>
#include <QKeyEvent>

PromptDialog::PromptDialog()
{
    QDesktopWidget dw;
    resize(QSize(dw.width() / 2, dw.height() / 2));

    QString style =\
            "QDialog {\
                border: 3px solid #63B8FF;\
                border-radius: 10px;\
                background-color: #FFFFFF;\
            }";
    style +="QListWidget {\
                border: 0px;\
                background-color: #EDEDED;\
            }"\
                "QScrollBar {\
                width: 0px;\
                height: 0px;\
            }"\
            "QListWidget::item {\
                background-color: #545454; \
                border-radius: 8px;\
                color: #FFFFFF;\
                height: 40px;\
                border: 1px solid #FCFCFC;\
            }";
    this->setStyleSheet(style);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(15, 0, 15, 15);
    this->setLayout(mainLayout);

    QString stateBtn = "QPushButton{background-color: transparent;}";

    QHBoxLayout *modeLayout = new QHBoxLayout();
    modeLayout->addStretch();
    modeLabel = new QPushButton();
    modeLabel->setStyleSheet(stateBtn);
    modeLayout->addWidget(modeLabel);
    modeLabel->setFont(QFont("Microsoft YaHei", 10));
    //modeLabel->setAlignment(Qt::AlignCenter);
    modeLayout->addStretch();
    mainLayout->addLayout(modeLayout);

    devAddrLabel = new QLabel("这里显示设备地址");
    devAddrLabel->setToolTip("这里显示设备地址");
    devAddrLabel->setFont(QFont("Microsoft YaHei", 20, QFont::Bold));
    devAddrLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mainLayout->addWidget(devAddrLabel);

    QHBoxLayout *operLayout = new QHBoxLayout();
    movie = new QMovie(":/icon/autorun.gif");
    movie->setScaledSize(QSize(100, 100));
    movieLabel = new QLabel();
    movieLabel->setFixedWidth(100);
    operLayout->addWidget(movieLabel);
    movieLabel->setMovie(movie);
    msgIconlabel = new QLabel();
    msgIconlabel->setPixmap(QPixmap(":/icon/msg.png").scaled(QSize(50, 50), Qt::KeepAspectRatio));
    operLayout->addWidget(msgIconlabel);
    operationPromptsLabel = new QLabel("这里显示需要的操作");
    operLayout->addWidget(operationPromptsLabel);
    operationPromptsLabel->setToolTip("这里显示需要的操作");
    operationPromptsLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    operLayout->addStretch();
    mainLayout->addLayout(operLayout);

    QString clearBtnStyle = "QPushButton:hover {\
                                background-color: #7A8B8B;\
                                border: 0px solid #030303;\
                            }"\
                            "QPushButton:pressed {\
                                background-color: #6E6E6E;\
                                border: 0px solid #030303;\
                            }";
    clearBtnStyle += "QPushButton {\
                        border: 0px solid #68838B;\
                        border-radius: 10px;\
                        width: 40px;\
                        height: 40px;\
                     }";
    manualWidgetLayout = new QHBoxLayout();
    mainLayout->addLayout(manualWidgetLayout);

    resultRecord = new QListWidget();
    resultRecord->setToolTip("配置结果记录");
    mainLayout->addWidget(resultRecord);
    QHBoxLayout *relbLayout = new QHBoxLayout();
    mainLayout->addLayout(relbLayout);
    resultTotalLabel = new QLabel("总数:0");
    relbLayout->addWidget(resultTotalLabel);
    clearBtn = new QPushButton("清除");
    clearBtn->setMaximumWidth(60);
    clearBtn->setStyleSheet(clearBtnStyle);
    relbLayout->addWidget(clearBtn);
    connect(clearBtn, SIGNAL(clicked()), this, SLOT(clearResultRecord()));

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#FF0000"));
    helpInfoLabel = new QLabel("※配置失败的一些可能原因\n"\
                               "1.配置页面选择的设备类型与实际的设备类型不匹配，这时会显示(AT+SENSOR)命令配置错误。\n"\
                               "2.当前设备不支持此配置命令,如果错误多次出现于在同一命令说明原因在此。\n"\
                               "3.配置的参数异常，不在限定范围内。\n"\
                               "4.配置过程等待长时间后提示配置错误，传感器的串口线可能连接异常。");
    helpInfoLabel->setPalette(pa);
    mainLayout->addWidget(helpInfoLabel);
    helpInfoLabel->hide();

    QString exitBtnStyle =  "QPushButton:hover {\
                                background-color: #EBEBEB;\
                                border: 3px solid #DEDEDE;\
                             }"\
                            "QPushButton:pressed {\
                                background-color: #F8F8FF;\
                                border: 3px solid #EBEBEB;\
                            }";
    exitBtnStyle += "QPushButton {\
                        background-color: #AAAAAA;\
                        border: 3px solid #BDBDBD;\
                        border-radius: 6px;\
                        height: 30px;\
                    }";
    QHBoxLayout *exitLayout = new QHBoxLayout();
    exitLayout->addStretch();
    exitBtn = new QPushButton("取消配置/关闭窗口(ESC)");
    exitBtn->setStyleSheet(exitBtnStyle);
    exitLayout->addWidget(exitBtn);
    exitLayout->addStretch();
    mainLayout->addLayout(exitLayout);
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(closePrompt()));
}

PromptDialog::~PromptDialog()
{
    qDebug() << "~PromptDialog";
}

void PromptDialog::setDevAddr(const QString &text)
{
    devAddrLabel->setText(text);
}

void PromptDialog::setoperationPrompts(const QString &text, QFont *ft, QPalette *pa)
{
    if (ft) {
        operationPromptsLabel->setFont(*ft);
    }
    if (pa) {
        operationPromptsLabel->setPalette(*pa);
    }
    operationPromptsLabel->setText(text);

}

void PromptDialog::setWarning(const QString &text)
{
    QFont ft("Microsoft YaHei", 20);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#ffcc00"));
    setoperationPrompts(text, &ft, &pa);
    helpInfoLabel->hide();
    msgIconlabel->setPixmap(QPixmap(":/icon/warn.png").scaled(QSize(50, 50), Qt::KeepAspectRatio));
}

void PromptDialog::setInfomation(const QString &text)
{
    QFont ft("Microsoft YaHei", 20);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#000000"));
    setoperationPrompts(text, &ft, &pa);
    helpInfoLabel->hide();
    msgIconlabel->setPixmap(QPixmap(":/icon/msg.png").scaled(QSize(50, 50), Qt::KeepAspectRatio));
}

void PromptDialog::setError(const QString &text)
{
    QFont ft("Microsoft YaHei", 20);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#FF4040"));
    setoperationPrompts(text, &ft, &pa);
    addResultItem(devAddrLabel->text() + "|" + text);
    helpInfoLabel->show();
    msgIconlabel->setPixmap(QPixmap(":/icon/error.png").scaled(QSize(50, 50), Qt::KeepAspectRatio));
}

void PromptDialog::setSuccess(const QString &text)
{
    QFont ft("Microsoft YaHei", 20);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#00EE00"));
    setoperationPrompts(text, &ft, &pa);
    addResultItem(devAddrLabel->text() + "|" + text);
    helpInfoLabel->hide();
    msgIconlabel->setPixmap(QPixmap(":/icon/msg.png").scaled(QSize(50, 50), Qt::KeepAspectRatio));
}

void PromptDialog::showPrompt()
{
    if (this->isHidden()) {
        movie->start();
        this->exec();
    }
}

void PromptDialog::setMode(const QString &text)
{
   // modeLabel->setText(text);
}

void PromptDialog::setMovieSpeed(int p)
{
    movie->setSpeed(p);
}

void PromptDialog::addResultItem(const QString &text)
{
    resultRecord->insertItem(0,  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "<" + text + ">");
    resultTotalLabel->setText(QString("总数:%1").arg(resultRecord->count()));
    QListWidgetItem *item = resultRecord->item(0);
    if (item) {
        item->setToolTip(QString("这是第%1条记录").arg(resultRecord->count()));
    }
}

void PromptDialog::closeEvent(QCloseEvent *event)
{
    if (event) {
        closePrompt();
    }
}

void PromptDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        closePrompt();
    }
}

void PromptDialog::setSensorType(SensorData::sensorType sensorType)
{
    modeLabel->setText(SensorData::sensorCodeString(sensorType));
    if (sensorType == SensorData::EFS) {
        modeLabel->setIcon(QIcon(":/sensorIcon/efs.png"));
    }
    else if (sensorType == SensorData::PRC) {
        modeLabel->setIcon(QIcon(":/sensorIcon/prc.png"));
    }
    else if (sensorType == SensorData::THS) {
        modeLabel->setIcon(QIcon(":/sensorIcon/ths.png"));
    }
    else if (sensorType == SensorData::WPS) {
        modeLabel->setIcon(QIcon(":/sensorIcon/wps.png"));
    }
    else if (sensorType == SensorData::WDS) {
        modeLabel->setIcon(QIcon(":/sensorIcon/wds.png"));
    }
    else if (sensorType == SensorData::TES) {
        modeLabel->setIcon(QIcon(":/sensorIcon/tes.png"));
    }
    else {
        modeLabel->setIcon(QIcon(":/sensorIcon/common.png"));
    }
}

void PromptDialog::closePrompt()
{
    emit cannel();
    this->close();
    movie->stop();
}

void PromptDialog::clearResultRecord()
{
    resultRecord->clear();
}
