#include "outdoorwlhs.h"

OutdoorWLHS::OutdoorWLHS(QWidget *parent) : QWidget(parent)
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

    QRegExp intExp("^[0-9]*");
    QValidator *intvalidator = new QRegExpValidator(intExp, this);

    QLabel *lowthLabel = new QLabel("低阈值:");
    cfgLayout->addWidget(lowthLabel, 3, 0);
    lowthEdit = new QLineEdit();
    lowthEdit->setValidator(intvalidator);
    cfgLayout->addWidget(lowthEdit, 3, 1);

    QLabel *highthLabel = new QLabel("高阈值:");
    cfgLayout->addWidget(highthLabel, 4, 0);
    highthEdit = new QLineEdit();
    highthEdit->setValidator(intvalidator);
    cfgLayout->addWidget(highthEdit, 4, 1);

    QLabel *varthLabel = new QLabel("变化量:");
    cfgLayout->addWidget(varthLabel, 5, 0);
    varthEdit = new QLineEdit();
    varthEdit->setValidator(intvalidator);
    cfgLayout->addWidget(varthEdit, 5, 1);

    QLabel *sampleTimeLabel = new QLabel("采样时间:");
    cfgLayout->addWidget(sampleTimeLabel, 6, 0);
    sampleTimeEdit = new QLineEdit();
    sampleTimeEdit->setValidator(intvalidator);
    cfgLayout->addWidget(sampleTimeEdit, 6, 1);
    QLabel *sampleTimeUint = new QLabel("秒");
    cfgLayout->addWidget(sampleTimeUint, 6, 2);

    QHBoxLayout *hsLaybout = new QHBoxLayout();
    mainLayout->addLayout(hsLaybout);
    QLabel *hstLabel = new QLabel("上报时长:");
    cfgLayout->addWidget(hstLabel, 7, 0);
    upintervalBox = new QComboBox();
    upintervalBox->setEditable(true);
    upintervalEdit = upintervalBox->lineEdit();
    upintervalEdit->setValidator(intvalidator);
    upintervalBox->setMinimumWidth(100);
    for (int n = 0; n < upintervalPreset.count(); n++) {
        upintervalBox->addItem(QString("%1").arg(upintervalPreset[n]));
    }
    cfgLayout->addWidget(upintervalBox, 7, 1);
    QLabel *hstUint = new QLabel("秒");
    cfgLayout->addWidget(hstUint, 7, 2);

    QRegExp retExp("(1\\d|\\d|20)");
    QValidator *retvalidator = new QRegExpValidator(retExp, this);
    QLabel *rtLabel = new QLabel("重传次数:");
    hsLaybout->addWidget(rtLabel);
    reTransEdit = new QLineEdit();
    //reTransEdit->setMaximumWidth(60);
    reTransEdit->setValidator(retvalidator);
    reTransEdit->setToolTip("0~20");
    reTransEdit->setPlaceholderText("0~20");
    hsLaybout->addWidget(reTransEdit);

    QLabel *dmLabel = new QLabel("LCD显示:");
    hsLaybout->addWidget(dmLabel);
    displayModeBox = new QComboBox();
    displayModeBox->setMinimumWidth(100);
    displayModeBox->addItems(QStringList() << "整数" << "小数");
    hsLaybout->addWidget(displayModeBox);

    mainLayout->addStretch();
    eventRecordInit();
}

OutdoorWLHS::~OutdoorWLHS()
{
    qDebug() << "~OutdoorWLHS";
}

void OutdoorWLHS::setEnabled(bool b)
{
    sbase->setEnabled(b);
    deveuiEdit->setEnabled(b);
    appeuiEdit->setEnabled(b);
    appkeyEdit->setEnabled(b);
    lowthEdit->setEnabled(b);
    highthEdit->setEnabled(b);
    varthEdit->setEnabled(b);
    upintervalBox->setEnabled(b);
    reTransEdit->setEnabled(b);
    displayModeBox->setEnabled(b);
    sampleTimeEdit->setEnabled(b);
}

bool OutdoorWLHS::getConfig(OWLHConfig &owlh)
{
    owlh.st = this->sensorType;
    sbase->getConfig(owlh.base);
    owlh.deveui = deveuiEdit->text();
    owlh.appeui = appeuiEdit->text();
    owlh.appkey = appkeyEdit->text();
    owlh.lowth = lowthEdit->text().toFloat();
    owlh.highth = highthEdit->text().toFloat();
    owlh.variation = varthEdit->text().toFloat();
    owlh.sampletime = sampleTimeEdit->text().toUInt();
    owlh.upintervalTime = upintervalEdit->text().toUInt();
    owlh.retrans = reTransEdit->text().toInt();
    owlh.displayMode = (OWLHConfig::lcdDisplayMode)displayModeBox->currentIndex();

    return true;
}

bool OutdoorWLHS::setConfig(OWLHConfig &owlh)
{
    sbase->setConfig(owlh.base);
    deveuiEdit->setText(owlh.deveui);
    appeuiEdit->setText(owlh.appeui);
    appkeyEdit->setText(owlh.appkey);

    lowthEdit->setText(QString("%1").arg(owlh.lowth));
    highthEdit->setText(QString("%1").arg(owlh.highth));
    varthEdit->setText(QString("%1").arg(owlh.variation));
    sampleTimeEdit->setText(QString("%1").arg(owlh.sampletime));
    upintervalEdit->setText(QString("%1").arg(owlh.upintervalTime));
    reTransEdit->setText(QString("%1").arg(owlh.retrans));
    displayModeBox->setCurrentIndex(owlh.displayMode);

    return true;
}

bool OutdoorWLHS::cancelApply()
{
    getATcmd()->clear();

    return true;
}

bool OutdoorWLHS::apply()
{
    OWLHConfig cfg;

    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    getConfig(cfg);

    getATcmd()->addCmd(getATcmd()->enterATModeCmd(), 1000, 20);
    getATcmd()->addCmd(getATcmd()->setActiveModeCmd(cfg.base.wm));
    getATcmd()->addCmd(getATcmd()->setDevaddrCmd(cfg.base.addr));
    getATcmd()->addCmd(getATcmd()->setAppskeyCmd(cfg.base.appskey));
    getATcmd()->addCmd(getATcmd()->setNewskeyCmd(cfg.base.nwskey));
    //getATcmd()->addCmd(getATcmd()->setAppeuiCmd(cfg.appeui));
    //getATcmd()->addCmd(getATcmd()->setAppkeyCmd(cfg.appkey));
    getATcmd()->addCmd(getATcmd()->setChannel(cfg.base.channelNum));
    getATcmd()->addCmd(getATcmd()->setWorkParame(cfg.upintervalTime, cfg.retrans, cfg.sampletime));
    getATcmd()->addCmd(getATcmd()->setTreshord(SensorData::thresholdMp(cfg.st) * cfg.lowth,\
                                               SensorData::thresholdMp(cfg.st) *cfg.highth,\
                                               SensorData::thresholdMp(cfg.st) *cfg.variation));
    getATcmd()->addCmd(getATcmd()->setLcdDisplayMode(cfg.displayMode));
    getATcmd()->addCmd(getATcmd()->saveCmd());
    getATcmd()->addCmd(getATcmd()->exitATModeCmd());
    getATcmd()->addFinishFlag("Exit AT command mode now");

    getATcmd()->exec(QStringList() << "Send AT+CMD=1" << "into AT command mode");

    return true;
}

QString OutdoorWLHS::parserDevAddr(const QString &text)
{
    QJsonParseError err;

    QJsonObject addr = QJsonDocument::fromJson(QByteArray(text.toUtf8()), &err).object();
    if (err.error) {
        /* 如果扫秒到的不是二维码，那么可能扫描到的是条形码 */
        QString data = text;
        QRegExp addrReg("[^A-Fa-f0-9]");
        return data.remove(addrReg);
    }

    return addr["Addr"].toString();
}

QString OutdoorWLHS::getDevAddr()
{
    OWLHConfig owlh;

    sbase->getConfig(owlh.base);

    return owlh.base.addr;
}

bool OutdoorWLHS::setDevAddr(const QString &addr)
{
    return sbase->setDevAddr(addr);
}

bool OutdoorWLHS::isValidConfig(QString *err)
{
    OWLHConfig cfg;

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
        //if (err) (*err) = "APPEUI配置异常";
        //return false;
    }
    if (cfg.deveui.size() < 16) {
        //if (err) (*err) = "DEVEUI配置异常";
        //return false;
    }
    if (cfg.appkey.size() < 32) {
        //if (err) (*err) = "APPKEY配置异常";
        //return false;
    }

    return true;
}

void OutdoorWLHS::setDefConfig(SensorData::sensorType sensorType)
{
    OWLHConfig owlh;

    this->sensorType = sensorType;

    if (SensorData::OWDS == sensorType) {
        owlh.base.wm = SensorData::ABP;
        owlh.base.addr = "50000001";
        owlh.base.appskey = "44c60a4d148c719923b14a8f4d44da29";
        owlh.base.nwskey = "44c60a4d148c719923b14a8f4d44da29";
        owlh.base.channelNum = 10;

        owlh.deveui = "8800000000888888";
        owlh.appeui = "70b3d57ed0026342";
        owlh.appkey = "2b7e151628aed2a6abf7158809cf4f3c";

        owlh.st = sensorType;
        owlh.lowth = 0.10;
        owlh.highth = 5.0;
        owlh.variation = 0;

        owlh.sampletime = 60;
        owlh.upintervalTime = 7200;
        owlh.retrans = 2;

        owlh.displayMode = OWLHConfig::DISPLAYFLOAT;
    }
    else if (SensorData::OWPS == sensorType) {
        owlh.base.wm = SensorData::ABP;
        owlh.base.addr = "50000001";
        owlh.base.appskey = "43c60a4d148c719923b14a8f4d44da29";
        owlh.base.nwskey = "43c60a4d148c719923b14a8f4d44da29";
        owlh.base.channelNum = 10;

        owlh.deveui = "8800000000888888";
        owlh.appeui = "70b3d57ed0026342";
        owlh.appkey = "2b7e151628aed2a6abf7158809cf4f3c";

        owlh.st = sensorType;
        owlh.lowth = 0.10;
        owlh.highth = 1.20;
        owlh.variation = 0;

        owlh.sampletime = 60;
        owlh.upintervalTime = 7200;
        owlh.retrans = 2;

        owlh.displayMode = OWLHConfig::DISPLAYFLOAT;
    }
    setConfig(owlh);
}

bool OutdoorWLHS::presetIsModify(const QString &text)
{
    OWLHConfig cfgdata;
    OWLHConfig dbdata;

    this->getConfig(cfgdata);
    getDB()->selectProject(this->sensorType, text, dbdata);

    return cfgdata != dbdata;
}

bool OutdoorWLHS::saveAsPreset(const QString &name)
{
    OWLHConfig data;

    this->getConfig(data);

    return getDB()->saveProject(name, data);
}

bool OutdoorWLHS::loadPreset(const QString &name)
{
    OWLHConfig data;

    if (getDB()->selectProject(this->sensorType, name, data)) {
        this->setConfig(data);
    }

    return true;
}

void OutdoorWLHS::eventRecordInit()
{

}

bool OutdoorWLHS::installEventRecord(const QString &prefix)
{
    sbase->installEventRecord(prefix);
    EventRecord::addEventObj(deveuiEdit, this, EventRecord::objName(prefix, "DEVEUI"));
    EventRecord::addEventObj(appeuiEdit, this, EventRecord::objName(prefix, "APPEUI"));
    EventRecord::addEventObj(appkeyEdit, this, EventRecord::objName(prefix, "APPKEY"));
    EventRecord::addEventObj(lowthEdit, this, EventRecord::objName(prefix, "低阈值"));
    EventRecord::addEventObj(highthEdit, this, EventRecord::objName(prefix, "高阈值"));
    EventRecord::addEventObj(varthEdit, this, EventRecord::objName(prefix, "变化量"));
    EventRecord::addEventObj(sampleTimeEdit, this, EventRecord::objName(prefix, "采样时间"));
    EventRecord::addEventObj(upintervalEdit, this, EventRecord::objName(prefix, "上报时长"));
    EventRecord::addEventObj(reTransEdit, this, EventRecord::objName(prefix, "重传次数"));
    EventRecord::addEventObj(displayModeBox, this, EventRecord::objName(prefix, "显示模式"));

    return true;
}
