#include "electricalfiresensor.h"
#include <QDebug>

ElectricalFireSensor::ElectricalFireSensor(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *thisLayout = new QVBoxLayout();
    QWidget *mainWidget = new QWidget();
    mainLayout = new QVBoxLayout();
    mainWidget->setLayout(mainLayout);
    this->setLayout(thisLayout);
    thisLayout->addWidget(mainWidget);
    sbase = new sensorBaseConfig();
    mainLayout->addWidget(sbase);

    QRegExp hex32Exp("[0-9A-Fa-f]{,32}");
    QRegExp hex16Exp("[0-9A-Fa-f]{,16}");
    QValidator *hex32validator = new QRegExpValidator(hex32Exp, this);
    QValidator *hex16validator = new QRegExpValidator(hex16Exp, this);

    QGridLayout *cfgLayout = new QGridLayout();
    mainLayout->addLayout(cfgLayout);
    QLabel *duLabel = new QLabel("Dev EUI: ");
    cfgLayout->addWidget(duLabel, 0, 0);
    deveuiEdit = new QLineEdit();
    deveuiEdit->setToolTip("16位十六进制值");
    deveuiEdit->setValidator(hex16validator);
    cfgLayout->addWidget(deveuiEdit, 0, 1);

    QLabel *auLabel = new QLabel("APP EUI: ");
    cfgLayout->addWidget(auLabel, 1, 0);
    appeuiEdit = new QLineEdit();
    appeuiEdit->setToolTip("16位十六进制值");
    appeuiEdit->setValidator(hex16validator);
    cfgLayout->addWidget(appeuiEdit, 1, 1);

    QLabel *akLabel = new QLabel("APP KEY: ");
    cfgLayout->addWidget(akLabel, 2, 0);
    appkeyEdit = new QLineEdit();
    appkeyEdit->setToolTip("32位十六进制值");
    appkeyEdit->setValidator(hex32validator);
    cfgLayout->addWidget(appkeyEdit, 2, 1);

    QHBoxLayout *hsLaybout = new QHBoxLayout();
    mainLayout->addLayout(hsLaybout);
    QLabel *hstLabel = new QLabel("握手时间:");
    hsLaybout->addWidget(hstLabel);
    hsTimeEdit = new QComboBox();
    hsTimeEdit->setMinimumWidth(100);
    for (int n = 0; n < hsList.count(); n++) {
        hsTimeEdit->addItem(QString("%1").arg(hsList[n]));
    }
    hsLaybout->addWidget(hsTimeEdit);
    QLabel *hstUint = new QLabel("秒");
    hsLaybout->addWidget(hstUint);

    QRegExp hsnExp("(1\\d|\\d|20)");
    QValidator *hsnvalidator = new QRegExpValidator(hsnExp, this);
    hsNumEdit = new QLineEdit();
    hsNumEdit->setValidator(hsnvalidator);
    hsNumEdit->setMaximumWidth(60);
    hsNumEdit->setToolTip("0~20");
    hsNumEdit->setPlaceholderText("0~20");
    hsLaybout->addStretch();

    QRegExp retExp("(1\\d|\\d|20)");
    QValidator *retvalidator = new QRegExpValidator(retExp, this);
    QLabel *rtLabel = new QLabel("重传次数:");
    hsLaybout->addWidget(rtLabel);
    reTransEdit = new QLineEdit();
    reTransEdit->setMaximumWidth(60);
    reTransEdit->setValidator(retvalidator);
    reTransEdit->setToolTip("0~20");
    reTransEdit->setPlaceholderText("0~20");
    hsLaybout->addWidget(reTransEdit);

    QRegExp tempExp("([4-9]{1}\\d{1}|1[0-4]{1}\\d{1})");
    QValidator *tempvalidator = new QRegExpValidator(tempExp, this);
    QHBoxLayout *temLaybout = new QHBoxLayout();
    mainLayout->addLayout(temLaybout);
    QLabel *temThLabel = new QLabel("温度阈值:");
    temLaybout->addWidget(temThLabel);
    channelTh1Edit = new QLineEdit();
    channelTh1Edit->setValidator(tempvalidator);
    channelTh1Edit->setMaximumWidth(60);
    channelTh1Edit->setToolTip("第一路: 45~140");
    channelTh1Edit->setPlaceholderText("45~140");
    temLaybout->addWidget(channelTh1Edit);
    channelTh2Edit = new QLineEdit();
    channelTh2Edit->setValidator(tempvalidator);
    channelTh2Edit->setMaximumWidth(60);
    channelTh2Edit->setToolTip("第二路: 45~140");
    channelTh2Edit->setPlaceholderText("45~140");
    temLaybout->addWidget(channelTh2Edit);
    channelTh3Edit = new QLineEdit();
    channelTh3Edit->setValidator(tempvalidator);
    channelTh3Edit->setMaximumWidth(60);
    channelTh3Edit->setToolTip("第三路: 45~140");
    channelTh3Edit->setPlaceholderText("45~140");
    temLaybout->addWidget(channelTh3Edit);
    channelTh4Edit = new QLineEdit();
    channelTh4Edit->setMaximumWidth(60);
    channelTh4Edit->setValidator(tempvalidator);
    channelTh4Edit->setToolTip("第四路: 45~140");
    channelTh4Edit->setPlaceholderText("45~140");
    temLaybout->addWidget(channelTh4Edit);
    QLabel *temUnitLabel = new QLabel("℃");
    temLaybout->addWidget(temUnitLabel);
    temLaybout->addStretch();

    QHBoxLayout *cLaybout = new QHBoxLayout();
    mainLayout->addLayout(cLaybout);
    QLabel *curThLabel = new QLabel("电流阈值:");
    cLaybout->addWidget(curThLabel);
    curthEdit = new QLineEdit();
    curthEdit->setMaximumWidth(80);
    curthEdit->setToolTip("0~互感器最大量程");
    curthEdit->setPlaceholderText("0~max");
    cLaybout->addWidget(curthEdit);
    QLabel *curUnitLabel = new QLabel("A");
    cLaybout->addWidget(curUnitLabel);

    cLaybout->addStretch();
    QRegExp prcExp("([2-9]{1}\\d{1}|\\d{3}|1[0]{3})");
    QValidator *prcvalidator = new QRegExpValidator(prcExp, this);
    QLabel *prcThLabel = new QLabel("剩余电流阈值:");
    cLaybout->addWidget(prcThLabel);
    prcthEdit = new QLineEdit();
    prcthEdit->setMaximumWidth(80);
    prcthEdit->setValidator(prcvalidator);
    prcthEdit->setToolTip("20~1000");
    prcthEdit->setPlaceholderText("20~1000");
    cLaybout->addWidget(prcthEdit);
    QLabel *prcUnitLabel = new QLabel("mA");
    cLaybout->addWidget(prcUnitLabel);

    QHBoxLayout *rwLaybout = new QHBoxLayout();
    mainLayout->addLayout(rwLaybout);
    QLabel *ratioLabel = new QLabel("电流互感器变比:");
    rwLaybout->addWidget(ratioLabel);
    ratioEdit = new QComboBox();
    ratioEdit->setMinimumWidth(130);
    for (int n = 0; n < ratioList.count(); n++) {
        ratioEdit->addItem(ratioList[n]);
    }
    rwLaybout->addWidget(ratioEdit);

    rwLaybout->addStretch();
    QLabel *wireLabel = new QLabel("传感器线制设置:");
    rwLaybout->addWidget(wireLabel);
    wireEdit = new QComboBox();
    wireEdit->setMinimumWidth(130);
    for (int n = 0; n < wireList.count(); n++) {
        wireEdit->addItem(wireList[n]);
    }
    rwLaybout->addWidget(wireEdit);

    QString mowstyle = ".QWidget {\
                        border:0px solid #63B8FF;\
                        border-radius: 10px;\
                     }";
    mowstyle += "QPushButton:hover {\
                 background-color: #EBEBEB;\
                 border: 3px solid #DEDEDE;\
                }"\
                "QPushButton:pressed {\
                  background-color: #F8F8FF;\
                  border: 3px solid #EBEBEB;\
                 }";
    mowstyle += "QPushButton {\
                  background-color: #AAAAAA;\
                  border: 3px solid #BDBDBD;\
                  border-radius: 6px;\
                  height: 30px;\
                }";

    manualOperWidget = new QWidget();
    manualOperWidget->setStyleSheet(mowstyle);
    mainLayout->addWidget(manualOperWidget);
    QVBoxLayout *manualLayout = new QVBoxLayout();
    manualLayout->setContentsMargins(0, 0, 0, 0);
    manualOperWidget->setLayout(manualLayout);

    QHBoxLayout *vpgLayout = new QHBoxLayout();
    manualLayout->addLayout(vpgLayout);
    enterATmode = new QPushButton("进入AT模式");
    EventRecord::addEventObj(enterATmode, this, "进入AT模式");
    vpgLayout->addWidget(enterATmode);
    connect(enterATmode, SIGNAL(clicked()), this, SLOT(enterAtModeBtnSlot()));

    vpgainBtn = new QPushButton("电压校准");
    EventRecord::addEventObj(vpgainBtn, this, "电压校准");
    vpgLayout->addWidget(vpgainBtn);
    connect(vpgainBtn, SIGNAL(clicked()), this, SLOT(vpgainBtnSlot()));

    QHBoxLayout *gainLayout = new QHBoxLayout();
    manualLayout->addLayout(gainLayout);
    cpgainBtn = new QPushButton("电流校准");
    EventRecord::addEventObj(cpgainBtn, this, "电流校准");
    gainLayout->addWidget(cpgainBtn);
    connect(cpgainBtn, SIGNAL(clicked()), this, SLOT(cpgainBtnSlot()));

    cpoffsetBtn = new QPushButton("电流校零");
    EventRecord::addEventObj(cpoffsetBtn, this, "电流校零");
    gainLayout->addWidget(cpoffsetBtn);
    connect(cpoffsetBtn, SIGNAL(clicked()), this, SLOT(cpoffsetBtnSlot()));

    QHBoxLayout *offsetLayout = new QHBoxLayout();
    manualLayout->addLayout(offsetLayout);
    cngainBtn = new QPushButton("剩余电流校准");
    EventRecord::addEventObj(cngainBtn, this, "剩余电流校准");
    offsetLayout->addWidget(cngainBtn);
    connect(cngainBtn, SIGNAL(clicked()), this, SLOT(cngainBtnSlot()));

    cnoffsetBtn = new QPushButton("剩余电流校零");
    EventRecord::addEventObj(cnoffsetBtn, this, "剩余电流校零");
    offsetLayout->addWidget(cnoffsetBtn);
    connect(cnoffsetBtn, SIGNAL(clicked()), this, SLOT(cnoffsetBtnSlot()));

    steptips = new StepTips(this);
    steptips->addStep(enterATmode, enterATmode->text(), enterATmode->text() + "(执行中...)", "等待手动复位设备后进入AT模式");
    steptips->addStep(vpgainBtn, vpgainBtn->text(), vpgainBtn->text() + "(执行中...)", "等待电压校准");
    steptips->addStep(cpgainBtn, cpgainBtn->text(), cpgainBtn->text() + "(执行中...)", "等待电流校准");
    steptips->addStep(cpoffsetBtn, cpoffsetBtn->text(), cpoffsetBtn->text() + "(执行中...)", "等待电流校零");
    steptips->addStep(cngainBtn, cngainBtn->text(), cngainBtn->text() + "(执行中...)", "等待剩余电流校准");
    steptips->addStep(cnoffsetBtn, cnoffsetBtn->text(), cnoffsetBtn->text() + "(执行中...)", "等待剩余电流校零");

    mainLayout->addStretch();
 }

ElectricalFireSensor::~ElectricalFireSensor()
{
    qDebug() << "~ElectricalFireSensor()";
}

void ElectricalFireSensor::setEnabled(bool b)
{
    sbase->setEnabled(b);
    deveuiEdit->setEnabled(b);
    appeuiEdit->setEnabled(b);
    appkeyEdit->setEnabled(b);
    hsTimeEdit->setEnabled(b);
    hsNumEdit->setEnabled(b);
    reTransEdit->setEnabled(b);
    channelTh1Edit->setEnabled(b);
    channelTh2Edit->setEnabled(b);
    channelTh3Edit->setEnabled(b);
    channelTh4Edit->setEnabled(b);
    curthEdit->setEnabled(b);
    prcthEdit->setEnabled(b);
    ratioEdit->setEnabled(b);
    wireEdit->setEnabled(b);
    vpgainBtn->setEnabled(b);
    cpgainBtn->setEnabled(b);
    cngainBtn->setEnabled(b);
    cpoffsetBtn->setEnabled(b);
    cnoffsetBtn->setEnabled(b);
}

bool ElectricalFireSensor::getConfig(efsConfig &cfg)
{
    sbase->getConfig(cfg.base);
    cfg.deveui = deveuiEdit->text();
    cfg.appeui = appeuiEdit->text();
    cfg.appkey = appkeyEdit->text();
    cfg.hstime = hsTimeEdit->currentIndex();
    cfg.hsnum = hsNumEdit->text().toUInt();
    cfg.retrans = reTransEdit->text().toUInt();
    cfg.tempth[0] = channelTh1Edit->text().toUInt();
    cfg.tempth[1] = channelTh2Edit->text().toUInt();
    cfg.tempth[2] = channelTh3Edit->text().toUInt();
    cfg.tempth[3] = channelTh4Edit->text().toUInt();
    cfg.curth = curthEdit->text().toUInt();
    cfg.prcth = prcthEdit->text().toUInt();
    cfg.ratio = ratioEdit->currentIndex();
    cfg.wrie = wireEdit->currentIndex();

    return true;
}

bool ElectricalFireSensor::setConfig(efsConfig &cfg)
{
    sbase->setConfig(cfg.base);
    deveuiEdit->setText(cfg.deveui);
    appeuiEdit->setText(cfg.appeui);
    appkeyEdit->setText(cfg.appkey);
    hsTimeEdit->setCurrentIndex(cfg.hstime);
    hsNumEdit->setText(QString("%1").arg(cfg.hsnum));
    reTransEdit->setText(QString("%1").arg(cfg.retrans));

    channelTh1Edit->setText(QString("%1").arg(cfg.tempth[0]));
    channelTh2Edit->setText(QString("%1").arg(cfg.tempth[1]));
    channelTh3Edit->setText(QString("%1").arg(cfg.tempth[2]));
    channelTh4Edit->setText(QString("%1").arg(cfg.tempth[3]));

    curthEdit->setText(QString("%1").arg(cfg.curth));
    prcthEdit->setText(QString("%1").arg(cfg.prcth));
    ratioEdit->setCurrentIndex(cfg.ratio);
    wireEdit->setCurrentIndex(cfg.wrie);

    return true;
}

bool ElectricalFireSensor::cancelApply()
{
    getATcmd()->clear();
    if (inManualOperation) {
        inManualOperation = false;
        steptips->hideTips();
        steptips->reset();
    }

    return true;
}

bool ElectricalFireSensor::apply()
{
    efsConfig cfg;

    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    getConfig(cfg);

    getATcmd()->addCmd(getATcmd()->enterATModeCmd(), 1000, 20);
    //cmd.addCmd(cmd.getCmdSupportCmd());
    //cmd.addCmd(cmd.setActiveModeCmd(cfg.base.wm));
    getATcmd()->addCmd(getATcmd()->setDevaddrCmd(cfg.base.addr));
    getATcmd()->addCmd(getATcmd()->setAppskeyCmd(cfg.base.appskey));
    getATcmd()->addCmd(getATcmd()->setNewskeyCmd(cfg.base.nwskey));
    getATcmd()->addCmd(getATcmd()->setAppeuiCmd(cfg.appeui));
    getATcmd()->addCmd(getATcmd()->setAppkeyCmd(cfg.appkey));
    getATcmd()->addCmd(getATcmd()->setChannel(cfg.base.channelNum));
    getATcmd()->addCmd(getATcmd()->setModeCmd(cfg.hstime, cfg.retrans));
    getATcmd()->addCmd(getATcmd()->setTemtresholdCmd(cfg.tempth[0], cfg.tempth[1], cfg.tempth[2], cfg.tempth[3]));
    getATcmd()->addCmd(getATcmd()->setCurtresholdCmd(cfg.curth));
    getATcmd()->addCmd(getATcmd()->setPrctresholdCmd(cfg.prcth));
    //cmd.addCmd(cmd.setRatioCmd(cfg.ratio));
   // cmd.addCmd(cmd.setWireCmd(cfg.wrie));
    getATcmd()->addCmd(getATcmd()->saveCmd());
    getATcmd()->addCmd(getATcmd()->exitATModeCmd());
    getATcmd()->addFinishFlag("Exit AT command mode now");

    getATcmd()->exec(QStringList() << "Send AT+CMD=1" << "into AT command mode");

    return true;
}

QString ElectricalFireSensor::parserDevAddr(const QString &text)
{
    QString data = text;
    QRegExp addrReg("[^A-Fa-f0-9]");
    return data.remove(addrReg);
}

QString ElectricalFireSensor::getDevAddr()
{
    efsConfig cfg;
    sbase->getConfig(cfg.base);

    return cfg.base.addr;
}

bool ElectricalFireSensor::setDevAddr(const QString &addr)
{
    if (addr.size() == DEVADDR_LENGTH) {
        sbase->setDevAddr(addr);
        return true;
    }
    return false;
}

bool ElectricalFireSensor::isValidConfig(QString *err)
{
    efsConfig cfg;

    getConfig(cfg);
    if (cfg.base.addr.size() < 8) {
        if (err) (*err) = "设备地址配置异常";
        return false;
    }
    if (cfg.base.appskey.size() < 32) {
        if (err) (*err) = "APPSKEY配置异常";
        return false;
    }
    if (cfg.base.nwskey.size() < 32) {
        if (err) (*err) = "MEWSKEY配置异常";
        return false;
    }
    if (cfg.appeui.size() < 16) {
        if (err) (*err) = "APPEUI配置异常";
        return false;
    }
    if (cfg.deveui.size() < 16) {
        if (err) (*err) = "DEVEUI配置异常";
        return false;
    }
    if (cfg.appkey.size() < 32) {
        if (err) (*err) = "APPKEY配置异常";
        return false;
    }
    if (cfg.tempth[0] > 140 || cfg.tempth[0] < 45) {
        if (err) (*err) = "第一路温度阈值配置异常";
        return false;
    }
    if (cfg.tempth[1] > 140 || cfg.tempth[1] < 45) {
        if (err) (*err) = "第二路温度阈值配置异常";
        return false;
    }
    if (cfg.tempth[2] > 140 || cfg.tempth[2] < 45) {
        if (err) (*err) = "第三路温度阈值配置异常";
        return false;
    }
    if (cfg.tempth[3] > 140 || cfg.tempth[3] < 45) {
        if (err) (*err) = "第四路温度阈值配置异常";
        return false;
    }
    if (cfg.tempth[0] > 140 || cfg.tempth[0] < 45) {
        if (err) (*err) = "第一路温度阈值配置异常";
        return false;
    }
    if (cfg.tempth[1] > 140 || cfg.tempth[1] < 45) {
        if (err) (*err) = "第二路温度阈值配置异常";
        return false;
    }
    if (cfg.tempth[2] > 140 || cfg.tempth[2] < 45) {
        if (err) (*err) = "第三路温度阈值配置异常";
        return false;
    }
    if (cfg.prcth < 20 || cfg.prcth > 1000) {
        if (err) (*err) = "剩余电流阈值设置异常";
        return false;
    }

    return true;
}

QWidget *ElectricalFireSensor::getManualWidget()
{
    return manualOperWidget;
}

void ElectricalFireSensor::setManualWidget()
{
    mainLayout->addWidget(manualOperWidget);
}

void ElectricalFireSensor::setDefConfig(SensorData::sensorType sensorType)
{
    efsConfig cfg;

    cfg.base.wm = SensorData::ABP;
    cfg.base.appskey = "50c60a4d148c719923b14a8f4d44da29";
    cfg.base.nwskey = "50c60a4d148c719923b14a8f4d44da29";
    cfg.base.channelNum = 10;
    cfg.deveui = "8800000000888888";
    cfg.appeui = "70b3d57ed0026342";
    cfg.appkey = "2b7e151628aed2a6abf7158809cf4f3c";
    cfg.hstime = 0;
    cfg.hsnum = 2;
    cfg.retrans = 2;
    cfg.tempth[0] = 70;
    cfg.tempth[1] = 70;
    cfg.tempth[2] = 70;
    cfg.tempth[3] = 50;
    cfg.curth = 75;
    cfg.prcth = 300;
    cfg.ratio = 0;
    cfg.wrie = 1;
    setConfig(cfg);
}

bool ElectricalFireSensor::presetIsModify(const QString &text)
{
    efsConfig cfgdata;
    efsConfig dbdata;

    this->getConfig(cfgdata);
    getDB()->selectProject(text, dbdata);

    return cfgdata != dbdata;
}

void ElectricalFireSensor::connectAtCmdFinish()
{
    connect(getATcmd(), SIGNAL(finish(ATCmd::finishState)), this, SLOT(atCmdFinish(ATCmd::finishState)));
}

bool ElectricalFireSensor::saveAsPreset(const QString &name)
{
    efsConfig data;

    this->getConfig(data);

    return getDB()->saveProject(name, data);
}

bool ElectricalFireSensor::loadPreset(const QString &name)
{
    efsConfig data;

    if (getDB()->selectProject(name, data)) {
        this->setConfig(data);
        return true;
    }

    return false;
}

bool ElectricalFireSensor::installEventRecord(const QString &prefix)
{
    sbase->installEventRecord(prefix);
    EventRecord::addEventObj(deveuiEdit, this, EventRecord::objName(prefix, "DEVEUI"));
    EventRecord::addEventObj(appeuiEdit, this, EventRecord::objName(prefix, "APPEUI"));
    EventRecord::addEventObj(appkeyEdit, this, EventRecord::objName(prefix, "APPKEY"));
    EventRecord::addEventObj(hsTimeEdit, this, EventRecord::objName(prefix, "握手时间"));
    EventRecord::addEventObj(reTransEdit, this, EventRecord::objName(prefix, "重传次数"));
    EventRecord::addEventObj(channelTh1Edit, this, EventRecord::objName(prefix, "第一路温度阈值"));
    EventRecord::addEventObj(channelTh2Edit, this, EventRecord::objName(prefix, "第二路温度阈值"));
    EventRecord::addEventObj(channelTh3Edit, this, EventRecord::objName(prefix, "第三路温度阈值"));
    EventRecord::addEventObj(channelTh4Edit, this, EventRecord::objName(prefix, "第四路温度阈值"));
    EventRecord::addEventObj(curthEdit, this, EventRecord::objName(prefix, "电流阈值"));
    EventRecord::addEventObj(prcthEdit, this, EventRecord::objName(prefix, "剩余电流阈值"));
    EventRecord::addEventObj(ratioEdit, this, EventRecord::objName(prefix, "电流互感变化"));
    EventRecord::addEventObj(wireEdit, this, EventRecord::objName(prefix, "传感器线制设置"));

    return true;
}

void ElectricalFireSensor::vpgainBtnSlot()
{
    if (!getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("三相电压校准"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return;
        }
    }
    if (QMessageBox::information(NULL, tr("三相电压校准"), tr("点击确认后开始执行操作!"), tr("确认"), tr("取消"), 0, 1)) {
        return;
    }
    connectAtCmdFinish();
    inManualOperation = true;
    getATcmd()->clear();
    getATcmd()->addCmd(getATcmd()->setVpgain(), 10000);
    getATcmd()->exec();
    hintMessage = "三相电压校准";
    steptips->highlightWidget(vpgainBtn);
    steptips->showTips();
}

void ElectricalFireSensor::cpgainBtnSlot()
{
    if (!getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("三相电流校准"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return;
        }
    }
    if (QMessageBox::information(NULL, tr("三相电流校准"), tr("点击确认后开始执行操作!"), tr("确认"), tr("取消"), 0, 1)) {
        return;
    }
    connectAtCmdFinish();
    inManualOperation = true;
    getATcmd()->clear();
    getATcmd()->addCmd(getATcmd()->setCpgain(), 10000);
    getATcmd()->exec();
    hintMessage = "三相电流校准";
    steptips->highlightWidget(cpgainBtn);
    steptips->showTips();
}

void ElectricalFireSensor::cngainBtnSlot()
{
    if (!getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("剩余电流校准"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return;
        }
    }
    if (QMessageBox::information(NULL, tr("剩余电流校准"), tr("点击确认后开始执行操作!"), tr("确认"), tr("取消"), 0, 1)) {
        return;
    }
    connectAtCmdFinish();
    inManualOperation = true;
    getATcmd()->clear();
    getATcmd()->addCmd(getATcmd()->setCNgain(), 10000);
    getATcmd()->exec();
    hintMessage = "剩余电流校准";
    steptips->highlightWidget(cngainBtn);
    steptips->showTips();
}

void ElectricalFireSensor::cpoffsetBtnSlot()
{
    if (!getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("三相电流校零"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return;
        }
    }
    if (QMessageBox::information(NULL, tr("三相电流校零"), tr("点击确认后开始执行操作!"), tr("确认"), tr("取消"), 0, 1)) {
        return;
    }
    connectAtCmdFinish();
    inManualOperation = true;
    getATcmd()->clear();
    getATcmd()->addCmd(getATcmd()->setCpoffset(), 10000);
    getATcmd()->exec();
    hintMessage = "三相电流校零";
    steptips->highlightWidget(cpoffsetBtn);
    steptips->showTips();
}

void ElectricalFireSensor::cnoffsetBtnSlot()
{
    if (!getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("剩余电流校零"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return;
        }
    }
    if (QMessageBox::information(NULL, tr("剩余电流校零"), tr("点击确认后开始执行操作!"), tr("确认"), tr("取消"), 0, 1)) {
        return;
    }
    connectAtCmdFinish();
    inManualOperation = true;
    getATcmd()->clear();
    getATcmd()->addCmd(getATcmd()->setCnoffset(), 10000);
    getATcmd()->exec();
    hintMessage = "剩余电流校零";
    steptips->highlightWidget(cnoffsetBtn);
    steptips->showTips();
}

void ElectricalFireSensor::enterAtModeBtnSlot()
{
    if (!getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("进入AT模式"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return;
        }
    }
    if (QMessageBox::information(NULL, tr("进入AT模式"), tr("点击确认后开始执行操作!"), tr("确认"), tr("取消"), 0, 1)) {
        return;
    }
    connectAtCmdFinish();
    inManualOperation = true;
    getATcmd()->clear();
    getATcmd()->addCmd(getATcmd()->enterATModeCmd(), 1000, 20);
    getATcmd()->exec(QStringList() << "Send AT+CMD=1" << "into AT command mode");
    hintMessage = "进入AT模式";
    steptips->reset();
    steptips->showTips();
}

void ElectricalFireSensor::atCmdFinish(ATCmd::finishState state)
{
    if (!inManualOperation) {
        return ;
    }
    inManualOperation = false;
    if (state == ATCmd::SUCCESS) {
        QMessageBox::information(NULL, tr("手动配置"), hintMessage + "成功", tr("是"));
        steptips->next();
    }
    else {
        steptips->hideTips();
        steptips->prev();
        QMessageBox::warning(NULL, tr("手动配置"), hintMessage + "失败", tr("是"));
    }
}
