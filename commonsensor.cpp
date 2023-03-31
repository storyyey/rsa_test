#include "commonsensor.h"

CommonSensor::CommonSensor(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *thisLayout = new QVBoxLayout();
    mainWidget = new QWidget();
    mainLayout = new QVBoxLayout();
    mainWidget->setLayout(mainLayout);
    this->setLayout(thisLayout);
    thisLayout->addWidget(mainWidget);
    baseWdigetInit();
    thresholdWidgetInit();
    sleepWidgetInit();
    mainLayout->addStretch();
}

CommonSensor::~CommonSensor()
{
    qDebug() << "~CommonSensor()";
}
void CommonSensor::baseWdigetInit()
{
    sbaseWidget = new sensorBaseConfig();
    mainLayout->addWidget(sbaseWidget);
}

void CommonSensor::thresholdWidgetInit()
{
    QGridLayout *valSetLayout = new QGridLayout();
    mainLayout->addLayout(valSetLayout);

    QRegExp floatExp("^(\\d+)(\\.\\d+)?");
    QValidator *floatvalidator = new QRegExpValidator(floatExp, this);

    QLabel *lowthLabel = new QLabel("低阈值:  ");
    lowthWidget.append(lowthLabel);
    valSetLayout->addWidget(lowthLabel, 0, 0);
    lowthEdit = new QLineEdit();
    lowthEdit->setValidator(floatvalidator);
    lowthWidget.append(lowthEdit);
    valSetLayout->addWidget(lowthEdit, 0, 1);
    lowthUnitLabel = new QLabel("米");
    lowthWidget.append(lowthUnitLabel);
    valSetLayout->addWidget(lowthUnitLabel, 0, 2);

    lowhumidity = new QLineEdit();
    lowhumidity->setValidator(floatvalidator);
    lowhumidityWidget.append(lowhumidity);
    valSetLayout->addWidget(lowhumidity, 0, 3);
    lowhumidityLabel = new QLabel("%RH");
    lowhumidityWidget.append(lowhumidityLabel);
    valSetLayout->addWidget(lowhumidityLabel, 0, 4);

    QLabel *highthLabel = new QLabel("高阈值:  ");
    highthWidget.append(highthLabel);
    valSetLayout->addWidget(highthLabel, 1, 0);
    highthEdit = new QLineEdit();
    highthEdit->setValidator(floatvalidator);
    highthWidget.append(highthEdit);
    valSetLayout->addWidget(highthEdit, 1, 1);
    highthUnitLabel = new QLabel("米");
    highthWidget.append(highthUnitLabel);
    valSetLayout->addWidget(highthUnitLabel, 1, 2);

    highthumidity = new QLineEdit();
    highthumidity->setValidator(floatvalidator);
    highthumidityWidget.append(highthumidity);
    valSetLayout->addWidget(highthumidity, 1, 3);
    highthumidityLabel = new QLabel("%RH");
    highthumidityWidget.append(highthumidityLabel);
    valSetLayout->addWidget(highthumidityLabel, 1, 4);

    QLabel *variationLabel = new QLabel("变化量:  ");
    varWidget.append(variationLabel);
    valSetLayout->addWidget(variationLabel, 2, 0);
    variationEdit = new QLineEdit();
    variationEdit->setValidator(floatvalidator);
    varWidget.append(variationEdit);
    valSetLayout->addWidget(variationEdit, 2, 1);
    variationUnitLabel = new QLabel("米");
    varWidget.append(variationUnitLabel);
    valSetLayout->addWidget(variationUnitLabel, 2, 2);

}

void CommonSensor::sleepWidgetInit()
{
    QRegExp intExp("^[0-9]*");
    QValidator *intvalidator = new QRegExpValidator(intExp, this);

    QGridLayout *swLayout = new QGridLayout();
    mainLayout->addLayout(swLayout);
    QLabel *sampleLabel = new QLabel("采样时间:");
    swLayout->addWidget(sampleLabel, 0, 0);
    sampleEdit = new QLineEdit();
    sampleEdit->setValidator(intvalidator);
    swLayout->addWidget(sampleEdit, 0, 1);
    QLabel *sampleUnitLabel = new QLabel("秒");
    swLayout->addWidget(sampleUnitLabel, 0, 2);

    QLabel *upIntervalLabel = new QLabel("上报时长:");
    swLayout->addWidget(upIntervalLabel, 0, 3);
    upIntervalEdit = new QLineEdit();
    upIntervalEdit->setValidator(intvalidator);
    swLayout->addWidget(upIntervalEdit, 0, 4);
    QLabel *upIntervalUnitLabel = new QLabel("秒");
    swLayout->addWidget(upIntervalUnitLabel, 0, 5);

    QLabel *retransLabel = new QLabel("重传次数:");
    swLayout->addWidget(retransLabel, 1, 0);
    retransEdit = new QLineEdit();
    retransEdit->setValidator(intvalidator);
    swLayout->addWidget(retransEdit, 1, 1);
    QLabel *retransUnitLabel = new QLabel("次");
    swLayout->addWidget(retransUnitLabel, 1, 2);
}

bool CommonSensor::getConfig(cmsConfig &cfg)
{
    sbaseWidget->getConfig(cfg.base);
    cfg.lowtv = lowthEdit->text().toFloat();
    cfg.lowtvUnit = lowthUnitLabel->text();
    cfg.lowhumidity = lowhumidity->text().toFloat();
    cfg.hightv = highthEdit->text().toFloat();
    cfg.hightvUnit = highthUnitLabel->text();
    cfg.highhumidity = highthumidity->text().toFloat();
    cfg.variation = variationEdit->text().toFloat();
    cfg.variationUnit = variationUnitLabel->text();
    cfg.sampleTime = sampleEdit->text().toUInt();
    cfg.upTransTime = upIntervalEdit->text().toUInt();
    cfg.reTrans = retransEdit->text().toUInt();

    return true;
}

bool CommonSensor::setConfig(cmsConfig &cfg)
{
    sbaseWidget->setConfig(cfg.base);
    lowthEdit->setText(QString("%1").arg(cfg.lowtv));
    lowthUnitLabel->setText(cfg.lowtvUnit);
    lowhumidity->setText(QString("%1").arg(cfg.lowhumidity));
    highthEdit->setText(QString("%1").arg(cfg.hightv));
    highthUnitLabel->setText(cfg.hightvUnit);
    highthumidity->setText(QString("%1").arg(cfg.highhumidity));
    variationEdit->setText(QString("%1").arg(cfg.variation));
    variationUnitLabel->setText(cfg.variationUnit);
    sampleEdit->setText(QString("%1").arg(cfg.sampleTime));
    upIntervalEdit->setText(QString("%1").arg(cfg.upTransTime));
    retransEdit->setText(QString("%1").arg(cfg.reTrans));

    return true;
}

void CommonSensor::setEnabled(bool b)
{
    sbaseWidget->setEnabled(b);
    lowthEdit->setEnabled(b);
    highthEdit->setEnabled(b);
    variationEdit->setEnabled(b);
    sampleEdit->setEnabled(b);
    upIntervalEdit->setEnabled(b);
    retransEdit->setEnabled(b);
}

QString CommonSensor::parserDevAddr(const QString &text)
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

QString CommonSensor::getDevAddr()
{
    cmsConfig cfg;
    sbaseWidget->getConfig(cfg.base);

    return cfg.base.addr;
}

bool CommonSensor::setDevAddr(const QString &addr)
{
    if (addr.size() == DEVADDR_LENGTH) {
        sbaseWidget->setDevAddr(addr);
        return true;
    }
    return false;
}

bool CommonSensor::isValidConfig(QString *err)
{
    cmsConfig cfg;

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

    return true;
}

void CommonSensor::widgetsShow(QList<QWidget *> &ws)
{
    for (int index = 0; index < ws.size(); index ++) {
        if(ws.at(index)) {
            ws.at(index)->show();
        }
    }
}

void CommonSensor::widgetsHide(QList<QWidget *> &ws)
{
    for (int index = 0; index < ws.size(); index ++) {
        if(ws.at(index)) {
            ws.at(index)->hide();
        }
    }
}

bool CommonSensor::IsModify(SensorData::sensorType type, const QString &text)
{
    cmsConfig cfgdata;
    cmsConfig dbdata;

    getConfig(cfgdata);
    getDB()->selectProject(type, text, dbdata);

    return cfgdata != dbdata;
}

bool CommonSensor::saveAs(SensorData::sensorType type, const QString &name)
{
    cmsConfig data;

    this->getConfig(data);
    data.st = type;

    return getDB()->saveProject(name, data);
}

bool CommonSensor::load(SensorData::sensorType type, const QString &name)
{
    cmsConfig data;

    this->getConfig(data);
    if (getDB()->selectProject(type, name, data)) {
        setConfig(data);
    }

    return true;
}

void CommonSensor::LowThresholdEdit(bool b)
{
    if (b) {
        widgetsShow(lowthWidget);
    }
    else {
        widgetsHide(lowthWidget);
    }
}

void CommonSensor::HighThresholdEdit(bool b)
{
    if (b) {
        widgetsShow(highthWidget);
    }
    else {
        widgetsHide(highthWidget);
    }
}

void CommonSensor::humidityEdit(bool b)
{
    if (b) {
        widgetsShow(lowhumidityWidget);
        widgetsShow(highthumidityWidget);
    }
    else {
        widgetsHide(lowhumidityWidget);
        widgetsHide(highthumidityWidget);
    }
}

void CommonSensor::variationEnableEdit(bool b)
{
    if (b) {
        widgetsShow(varWidget);
    }
    else {
        widgetsHide(varWidget);
    }
}

bool CommonSensor::cancelApply()
{
    getATcmd()->clear();

    return true;
}

bool CommonSensor::apply()
{
    return false;
}

bool CommonSensor::installEventRecord(const QString &prefix)
{
    sbaseWidget->installEventRecord(prefix);
    EventRecord::addEventObj(lowthEdit, this, EventRecord::objName(prefix, "低阈值"));
    EventRecord::addEventObj(lowhumidity, this, EventRecord::objName(prefix, "湿度低阈值"));
    EventRecord::addEventObj(highthEdit, this, EventRecord::objName(prefix, "高阈值"));
    EventRecord::addEventObj(highthumidity, this, EventRecord::objName(prefix,"湿度高阈值"));
    EventRecord::addEventObj(variationEdit, this, EventRecord::objName(prefix, "变化量"));
    EventRecord::addEventObj(sampleEdit, this, EventRecord::objName(prefix, "采样时间"));
    EventRecord::addEventObj(upIntervalEdit, this, EventRecord::objName(prefix, "上报时间"));
    EventRecord::addEventObj(retransEdit, this, EventRecord::objName(prefix, "重传次数"));

    return true;
}
