#include "uartwidget.h"
#include <QDebug>
#include "mainwindow.h"

UartWidget::UartWidget(QWidget *parent) : QWidget(parent)
{
    QString comBoxstyle =\
            "QComboBox {\
                border:1px solid #63B8FF;\
                border-radius:4px;\
                height: 30px;\
            }";
    QString Style = ".QWidget {\
                background-color: #FAFAFA;\
                border-radius: 10px;\
             }";
    btnStyle = "QPushButton:hover {\
                background-color: #EBEBEB;\
             }"\
             "QPushButton:pressed {\
                background-color: #F8F8FF;\
             }";
    btnStyle += "QPushButton {\
                background-color: #FAFAFA;\
                border-radius: 10px;\
                height: 35px;\
              }";
    QString stateBtn = "QPushButton{background-color: transparent;}";

    this->setStyleSheet(comBoxstyle + Style);
    QVBoxLayout *thisLayout = new QVBoxLayout();
    this->setLayout(thisLayout);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QWidget *mWidget = new QWidget();
    mWidget->setLayout(mainLayout);
    thisLayout->addWidget(mWidget);

    QHBoxLayout *comlayout = new QHBoxLayout();
    mainLayout->addLayout(comlayout);

    QHBoxLayout *suLayout = new QHBoxLayout();
    sensorPortComBox = new QComboBox();
    sensorUartBtn =  new QPushButton("传感器");
    sensorUartBtn->setMaximumWidth(100);
    sensorUartBtn->setIcon(QIcon(":/icon/com.png"));
    sensorUartBtn->setToolTip("打开或者关闭传感器串口");
    sensorUartBtn->setStyleSheet(btnStyle);
    sensorPortState = new QPushButton();
    sensorPortState->setStyleSheet(stateBtn);
    sensorPortState->setIcon(QIcon(closeStatIcon));
    sensorPortState->setMaximumWidth(30);
    suLayout->addWidget(sensorPortState);
    suLayout->addWidget(sensorPortComBox);
    suLayout->addWidget(sensorUartBtn);
    comlayout->addLayout(suLayout);

    QHBoxLayout *buLayout = new QHBoxLayout();
    barcodePortComBox = new QComboBox();
    barcodeUartBtn =  new QPushButton("扫码枪");
    barcodeUartBtn->setMaximumWidth(100);
    barcodeUartBtn->setIcon(QIcon(":/icon/barcode.png"));
    barcodeUartBtn->setToolTip("打开或者关闭扫码枪串口");
    barcodeUartBtn->setStyleSheet(btnStyle);
    barcodePortState = new QPushButton();
    barcodePortState->setStyleSheet(stateBtn);
    barcodePortState->setIcon(QIcon(closeStatIcon));
    barcodePortState->setMaximumWidth(30);
    buLayout->addWidget(barcodePortState);
    buLayout->addWidget(barcodePortComBox);
    buLayout->addWidget(barcodeUartBtn);
    comlayout->addLayout(buLayout);

    helpDialogInit();
    QPushButton *helpBtn = new QPushButton();
    helpBtn->setMaximumWidth(30);
    helpBtn->setIcon(QIcon(":/icon/help.png"));
    comlayout->addWidget(helpBtn);
    helpBtn->setStyleSheet(stateBtn);
    QObject::connect(helpBtn, &QPushButton::clicked, this, [this, helpBtn]() {
        QPoint btnpos = helpBtn->pos();
        QSize btnsize = helpBtn->size();

        helpDialog->setGeometry(mapToGlobal(btnpos).x() + btnsize.width() * 2,\
                               mapToGlobal(btnpos).y() + btnsize.height() * 2, 700, 300);

        helpDialog->exec();
    });

    initNewDevTips();

    sensorSerial  = new QSerialPort(this);
    barcodeSerial = new QSerialPort(this);
    portListRefresh();

    refhPortTimer = new QTimer(this);
    connect(refhPortTimer, SIGNAL(timeout()), this, SLOT(autoRefreshPortTimer()));
    refhPortTimer->setInterval(8000);
    refhPortTimer->start();

    connect(sensorSerial, SIGNAL(readyRead()), this, SLOT(readSeneorInfo()));
    connect(barcodeSerial, SIGNAL(readyRead()), this, SLOT(readBarcodeInfo()));

    connect(sensorUartBtn, SIGNAL(clicked()), this, SLOT(sensorUartBtnClick()));
    connect(barcodeUartBtn, SIGNAL(clicked()), this, SLOT(barcodeUartBtnClick()));

    standStateTimer = new QTimer(this);
    connect(standStateTimer, &QTimer::timeout, this, [this]{
        static bool overturn = false;

        if (overturn) {
            if (sensorStandState) {
                sensorPortState->setIcon(QIcon(standStatIcon));
            }
            if (barcodeStandState) {
                barcodePortState->setIcon(QIcon(standStatIcon));
            }
        }
        else {
            if (sensorStandState) {
                sensorPortState->setIcon(QIcon(openStatIcon));
            }
            if (barcodeStandState) {
                barcodePortState->setIcon(QIcon(openStatIcon));
            }
        }
        overturn = !overturn;
    });
    standStateTimer->setInterval(1000);
    standStateTimer->start();

    sensorKeepTimer = new QTimer(this);
    connect(sensorKeepTimer, &QTimer::timeout, this, [this]{
        sensorStandHint(true);
    });
    sensorKeepTimer->setInterval(30000);

    barcodeKeepTimer = new QTimer(this);
    connect(barcodeKeepTimer, &QTimer::timeout, this, [this]{
        sensorStandHint(true);
    });
    barcodeKeepTimer->setInterval(30000);
}

UartWidget::~UartWidget()
{
    qDebug() << "~UartWidget";
}

void UartWidget::portListRefresh()
{
    sensorPortComBox->clear();
    barcodePortComBox->clear();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        if (!portList.contains(info.portName())) {
            portList.append(info.portName());
        }
        sensorPortComBox->addItem(info.portName());
        barcodePortComBox->addItem(info.portName());
    }
    if (!sensorPortComBox->count()) {
        sensorPortComBox->addItem("无");
    }
    if (!barcodePortComBox->count()) {
        barcodePortComBox->addItem("无");
    }
}

bool UartWidget::openPort(const QString &name, QSerialPort *serial)
{
    if (!serial) {
        return false;
    }
    closePort(serial);

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        if (info.portName() == name) {
            if (info.isBusy()) {
                windowConsole(QString("%1串口被占用").arg(name));
                QMessageBox::warning(NULL, "设备", QString("(%1)串口被占用").arg(name), "是");
                return false;
            }
        }
    }

    serial->setPortName(name);
    if (serial->open(QIODevice::ReadWrite)) {
        serial->setDataBits(QSerialPort::Data8);
        serial->setBaudRate(QSerialPort::Baud115200);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        windowConsole(QString("%1串口打开成功").arg(name));

        if (serial == sensorSerial) {
            sensorPortState->setIcon(QIcon(openStatIcon));
            sensorStandHint(true);
            sensorKeepTimer->start();
            sensorPortComBox->setCurrentText(name);
            sensorPortComBox->setDisabled(true);
        }
        else if (serial == barcodeSerial) {
            barcodePortState->setIcon(QIcon(openStatIcon));
            barcodeStandHint(true);
            barcodeKeepTimer->start();
            barcodePortComBox->setCurrentText(name);
            barcodePortComBox->setDisabled(true);
        }

        if (sensorSerial->isOpen() && barcodeSerial->isOpen()) {
            refhPortTimer->stop();
        }

        return true;
    }
    windowConsole(QString("%1串口打开失败").arg(name));

    return false;
}

bool UartWidget::closePort(QSerialPort *serial)
{
    if (!serial) {
        return false;
    }

    if (serial->isOpen()) {
        refhPortTimer->start();
        serial->close();
        windowConsole(QString("%1串口关闭成功").arg(serial->portName()));
    }

    if (serial == sensorSerial) {
        sensorPortState->setIcon(QIcon(closeStatIcon));
        sensorStandHint(false);
        sensorKeepTimer->stop();
        sensorPortComBox->setDisabled(false);
    }
    else if (serial == barcodeSerial) {
        barcodePortState->setIcon(QIcon(closeStatIcon));
        barcodeStandHint(false);
        barcodeKeepTimer->stop();
        barcodePortComBox->setDisabled(false);
    }

    return true;
}

void UartWidget::setATcmd(ATCmd *ac)
{
    this->ac = ac;
    connect(ac, SIGNAL(next(QString)), this, SLOT(sendToSensor(QString)));
}

void UartWidget::setCurrPortListText(const QString &name, QComboBox *box)
{
    for (int n = 0; n < box->count(); n++) {
        if (name == box->itemText(n)) {
            box->setCurrentIndex(n);
        }
    }
}

void UartWidget::setConfigProgressDialog(PromptDialog *cpd)
{
    this->configProgressDialog = cpd;
}

void UartWidget::initNewDevTips()
{
    newDevTips = new QDialog(this);
    QVBoxLayout *mlayout = new QVBoxLayout();
    newDevTips->setLayout(mlayout);
    tipsLabel = new QLabel();
    mlayout->addWidget(tipsLabel);

    QPushButton *barcodeBtn = new QPushButton("扫码枪");
    barcodeBtn->setIcon(QIcon(":/icon/barcode.png"));
    barcodeBtn->setStyleSheet(btnStyle);
    QObject::connect(barcodeBtn, &QPushButton::clicked, this, [this]() {
        if (this->barcodeSerial->isOpen()) {
            QMessageBox::warning(NULL, "扫码枪", "扫码枪端口已经打开，请重新确认选择是否正确", "知道了");
            return ;
        }
        this->openPort(this->newDevName, this->barcodeSerial);
        this->newDevTips->close();
    });
    mlayout->addWidget(barcodeBtn);
    QPushButton *sensroBtn = new QPushButton("传感器");
    sensroBtn->setIcon(QIcon(":/icon/com.png"));
    sensroBtn->setStyleSheet(btnStyle);
    QObject::connect(sensroBtn, &QPushButton::clicked, this, [this]() {
        if (this->sensorSerial->isOpen()) {
            QMessageBox::warning(NULL, "传感器", "传感器端口已经打开，请重新确认选择是否正确", "知道了");
            return ;
        }
        this->openPort(this->newDevName, this->sensorSerial);
        this->newDevTips->close();
    });
    mlayout->addWidget(sensroBtn);
    QPushButton *cancelBtn = new QPushButton("取消/关闭");
    cancelBtn->setStyleSheet(btnStyle);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, [this]() {
        this->newDevTips->close();
    });
    mlayout->addWidget(cancelBtn);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#FF0000"));
    QLabel *helpLabel = new QLabel("※如果选错了手动点击按钮关闭所有端口,\n""然后从电脑上移除设备等待10秒后重新接入设备");
    helpLabel->setPalette(pa);
    mlayout->addWidget(helpLabel);
}

void UartWidget::sensorStandHint(bool flag)
{
    sensorStandState = flag;
    if (!sensorStandState) {
        if (sensorSerial->isOpen()) {
            sensorPortState->setIcon(QIcon(openStatIcon));
        }
        else {
            sensorPortState->setIcon(QIcon(closeStatIcon));
        }
    }
}

void UartWidget::barcodeStandHint(bool flag)
{
    barcodeStandState = flag;
    if (!barcodeStandState) {
        if (barcodeSerial->isOpen()) {
            barcodePortState->setIcon(QIcon(openStatIcon));
        }
        else {
            barcodePortState->setIcon(QIcon(closeStatIcon));
        }
    }
}

void UartWidget::helpDialogInit()
{
    helpDialog = new QDialog(this);
    helpDialog->setWindowTitle("帮助文档");

    QScrollArea *scrollbar = new QScrollArea();
    scrollbar->setGeometry(0, 0, 600, 300);
    scrollbar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    helpDialog->setLayout(mainLayout);
    mainLayout->addWidget(scrollbar);
    QWidget *stepWidget = new QWidget();
    stepWidget->setGeometry(0, 0, 1000, 6000);
    scrollbar->setWidget(stepWidget);
    QVBoxLayout *stepmainLayout = new QVBoxLayout();
    QGridLayout *stepLayout = new QGridLayout();
    stepmainLayout->addLayout(stepLayout);
    stepmainLayout->addStretch();
    stepWidget->setLayout(stepmainLayout);

    QString titleStyle = "QLabel{"
                            "color: red;"
                            "font-size: 26px;"
                            "font-weight: bold;"
                         "}";

    QString stateBtnStyle = "QPushButton{background-color: transparent;}";

    QLabel *statIconLabel = new QLabel("1.串口状态指示");
    statIconLabel->setStyleSheet(titleStyle);
    stepLayout->addWidget(statIconLabel, 0, 0);

    QLabel *statExplain = new QLabel("红色: 串口关闭。\n"
                                     "黄绿: 串口已打开但一段时间内未接收到数据，会黄绿交替闪烁。\n"
                                     "绿色: 串口已打开并成功接收到数据。");
    stepLayout->addWidget(statExplain, 1, 0);

    QLabel *useExplain = new QLabel("2.如何找到正确的串口并打开?");
    useExplain->setStyleSheet(titleStyle);
    stepLayout->addWidget(useExplain, 2, 0);
    QLabel *newcomExplain = new QLabel("重新将串口线连接电脑后等待几秒后，会弹出确认选择窗口，\n"
                                       "即可正确打开串口。");
    stepLayout->addWidget(newcomExplain, 3, 0);
}

void UartWidget::sensorUartBtnClick()
{
    if (sensorSerial->isOpen()) {
        closePort(sensorSerial);
    }
    else {
        QString name = sensorPortComBox->currentText();
        openPort(name, sensorSerial);
    }
}

void UartWidget::barcodeUartBtnClick()
{
    if (barcodeSerial->isOpen()) {
        closePort(barcodeSerial);
    }
    else {
        QString name = barcodePortComBox->currentText();
        openPort(name, barcodeSerial);
    }
}

void UartWidget::readSeneorInfo()
{
    QByteArray respon;

    respon = sensorSerial->readAll();
    if (respon.isEmpty()) {
        return ;
    }
    if (QString(respon.toStdString().c_str()).remove(QRegExp("[\\w]")).size() > 3) {
        sensorKeepTimer->setInterval(30000);
        sensorStandHint(false);
    }

    windowConsole(QString("收(%1)%2").arg(respon.size()).arg(respon.toStdString().c_str()));
    if (ac->isExecTrigger(respon.toStdString().c_str())) {
        configProgressDialog->setInfomation("设备进入配置模式成功!");
        configProgressDialog->setInfomation("等待设备配置完成......");
    }
    else {
        ac->parserResponse(respon);
    }
}

void UartWidget::readBarcodeInfo()
{
    QByteArray respon;

    respon = barcodeSerial->readAll();
    if (respon.isEmpty()) {
        return ;
    }
    barcodeKeepTimer->setInterval(30000);
    barcodeStandHint(false);
    windowConsole(QString("扫码数据(%1)%2").arg(respon.size()).arg(respon.toStdString().c_str()));
    emit barcodeData(tr(respon));
}

void UartWidget::sendToSensor(const QString &cmd)
{
    if (sensorSerial->isOpen()) {
        sensorSerial->write(cmd.toUtf8());
        windowConsole(QString("发(%1)%2").arg(cmd.toUtf8().size()).arg(cmd.toUtf8().toStdString().c_str()));
    }
    else {
        windowConsole(QString("串口未打开发送失败"));
    }
}

void UartWidget::CmdFinish(ATCmd::finishState state)
{
    if (state == ATCmd::SUCCESS) {
        configProgressDialog->setSuccess("设备配置成功!");
    }
    else {
        configProgressDialog->setError("设备配置失败!");
    }

    windowConsole(QString("向传感器发送数据结束:%1").arg(state));
}

void UartWidget::autoRefreshPortTimer()
{
    QStringList newportList;
    refhPortTimer->stop();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        newportList.append(info.portName());
    }
    if (!sensorSerial->isOpen()) {
        sensorPortComBox->clear();
        sensorPortComBox->addItems(newportList);
    }
    else {
        if (!newportList.contains(sensorSerial->portName())) {
            closePort(sensorSerial);
            sensorPortComBox->clear();
            sensorPortComBox->addItems(newportList);
            QMessageBox::warning(NULL, tr("传感器"), "传感器异常断开", tr("是"));
        }
    }
    if (!barcodeSerial->isOpen()) {
        barcodePortComBox->clear();
        barcodePortComBox->addItems(newportList);
    }
    else {
        if (!newportList.contains(barcodeSerial->portName())) {
            closePort(barcodeSerial);
            barcodePortComBox->clear();
            barcodePortComBox->addItems(newportList);
            QMessageBox::warning(NULL, tr("传感器"), "扫码枪异常断开", tr("是"));
        }
    }
    for (int index = 0; index < newportList.size(); index++) {
        if (!portList.contains(newportList.at(index))) {
            newDevName = newportList.at(index);
            tipsLabel->setText(QString("发现新设备(%1)接入这个设备是什么?").arg(newDevName));
            newDevTips->exec();
            /* 新设备 */
        }
    }
    portList.clear();
    portList = newportList;

    if (!sensorPortComBox->count()) {
        sensorPortComBox->addItem("无");
    }
    if (!barcodePortComBox->count()) {
        barcodePortComBox->addItem("无");
    }

    refhPortTimer->start();
}
