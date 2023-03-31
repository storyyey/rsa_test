#include "sensorconfigwidget.h"
#include "settingOption.h"
#include "operationpanel.h"

SensorConfigWidget::SensorConfigWidget(QWidget *parent) : QWidget(parent)
{
    widgetInit();
}

SensorConfigWidget::~SensorConfigWidget()
{
    qDebug() << "~SensorConfigWidget()";
    savePreset->deleteLater();
}

SensorConfigWidget::SensorConfigWidget(TablesWidget *tables, DataBase *DB)
{
    if (tables && DB) {
        this->tables = tables;
        this->parentDB = DB;
        connect(DB, SIGNAL(selectConfigRecord(SensorData::configRecord &)), \
            tables, SLOT(selectConfigRecordSlot(SensorData::configRecord &)));
        DB->foreachConfigRecord();
    }
    widgetInit();
}

void SensorConfigWidget::widgetInit()
{
    QString style =\
            "QComboBox {\
                border:1px solid #63B8FF;\
                border-radius:4px;\
                height: 30px;\
             }";
    style += "QLineEdit {\
                border:1px solid #63B8FF;\
                border-radius:4px;\
                height: 30px;\
              }";
    style += ".QWidget {\
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
    savePreset = new SaveAsDialog();
    connect(savePreset, SIGNAL(saveAs(QString)), this, SLOT(saveAsConfirm(QString)));

    QWidget *mainWidge = new QWidget();
    mainLayout = new QVBoxLayout();
    mainWidge->setLayout(mainLayout);
    QVBoxLayout *thisLayout = new QVBoxLayout();
    this->setLayout(thisLayout);
    thisLayout->addWidget(mainWidge);

    QGridLayout *stLayout = new QGridLayout();
    mainLayout->addLayout(stLayout);
    QLabel *typeLabel = new QLabel("传感器类型:");
    typeLabel->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    stLayout->addWidget(typeLabel, 0, 0);
    sensorTypeCombox = new QComboBox();
    sensorTypeComboxRefsh();
    stLayout->addWidget(sensorTypeCombox, 0, 1);
    connect(sensorTypeCombox, &QComboBox::currentTextChanged,\
            this, &SensorConfigWidget::selectSensorChange);

    QLabel *pvLabel = new QLabel("预设方案:");
    pvLabel->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    stLayout->addWidget(pvLabel, 0, 2);
    presetValBox = new QComboBox();
    presetValBox->addItem(presetBoxDefVal);

    stLayout->addWidget(presetValBox, 0, 3);
    connect(presetValBox, SIGNAL(activated(int)),\
            this, SLOT(presetProjectChange(int)));
    eventRecordInit();
    typeStacke = new QStackedWidget();
    mainLayout->addWidget(typeStacke);

    sensorWidgetMapInit();
    setConfigMode(MANNUL);
    presetProjectResh();
    setini();
}

void SensorConfigWidget::sensorTypeComboxRefsh()
{
    QString Tips;
    for (int n = 0; n < sensorList.count(); n++) {
        sensorTypeCombox->addItem(SensorData::sensorCodeString(sensorList[n]));
        Tips += SensorData::sensorCodeString(sensorList[n]) + "\n";
    }
    sensorTypeCombox->setToolTip(Tips);
}

void SensorConfigWidget::setATcmd(ATCmd *ac)
{
    this->parentAtCmd = ac;
    for (int index = 0; index < sensorList.count(); index++) {
        SensorOperator[sensorList[index]]->setATcmd(ac);
    }
    connect(ac, SIGNAL(finish(ATCmd::finishState)), this, SLOT(atCmdFinish(ATCmd::finishState)));
}

void SensorConfigWidget::presetProjectResh()
{
    QStringList prolist;

    presetValBox->clear();
    presetValBox->addItem(presetBoxDefVal);
    if (parentDB) parentDB->selectProjectList(currType(), prolist);
    presetValBox->addItems(prolist);
}

void SensorConfigWidget::setPromptDialog(PromptDialog *cpd)
{
    this->prompt = cpd;
    connect(this->prompt, SIGNAL(cannel()), this, SLOT(cannelConfig()));
}

void SensorConfigWidget::newDevicesInfo(const QString &text)
{
    if (!parentAtCmd->isIdle()) {
        prompt->setDevAddr("配置时被扫码中断，请重新扫码开始配置!");
        prompt->setWarning("异常中断，配置中请勿扫码!");
        stopConfig();
        return ;
    }

    QString addr = SensorOperator[currType()]->parserDevAddr(text);
    if (!SensorOperator[currType()]->setDevAddr(addr)) {
        prompt->setWarning("扫描条码失败，请重新扫描！");
        return ;
    }
    if (getConfigMode() == AUTO) {
        startConfig(addr);
    }
}

void SensorConfigWidget::atCmdFinish(ATCmd::finishState state)
{
    SensorData::configRecord data;

    if (prompt->isHidden()) {
        return ;
    }
    if (state == ATCmd::SUCCESS) {
        prompt->setSuccess("配置成功!");
        data.state = true;
    }
    else {
        prompt->setError("配置失败!");
        data.state = false;
    }
    data.time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    data.cfile = presetValBox->currentText();
    data.addr = SensorOperator[currType()]->getDevAddr();
    data.type = currType();
    tables->addItem(data);
    parentDB->save(data);
    prompt->setDevAddr("等待扫描设备条码,获取设备地址");
    stopConfig();
}

void SensorConfigWidget::saveAsConfirm(const QString &name)
{
    SensorOperator[currType()]->saveAsPreset(name);
}

void SensorConfigWidget::activeSlot(int type)
{
    switch (type) {
        case OperationPanel::activePreview: {
            if (!this->parentAtCmd) {
                return ;
            }
            QDialog previewDialog;
            QHBoxLayout previewLayout;
            QPlainTextEdit previewList;

            EventRecord::readEventLog();

            previewDialog.setLayout(&previewLayout);
            previewLayout.addWidget(&previewList);
            previewDialog.setMinimumSize(QSize(800, 400));
            previewList.setFont(QFont("Microsoft YaHei", 12));
            SensorOperator[currType()]->apply();
            QStringList cl = this->parentAtCmd->cmdDump();
            for (int index = 0; index < cl.size(); index++) {
                previewList.insertPlainText("# " + parentAtCmd->description(cl.at(index)));
                previewList.insertPlainText("\r\n");
                previewList.insertPlainText(cl.at(index));
            }
            SensorOperator[currType()]->cancelApply();
            previewDialog.exec();
            break;
        }
        case OperationPanel::activeConfig:
            saveBtnSlot();
            break;

        case OperationPanel::activeSaveas:
            saveAsProject();
            break;
        case OperationPanel::activeAutoConfig:
            changeModeBtnSlot();
            break;
        default:
            break;
    }
}

void SensorConfigWidget::setConfigMode(SensorConfigWidget::ConfigMode mode)
{
    this->mode = mode;
    if (mode == MANNUL) {
        if (prompt) {prompt->setMode("单配置");}
        SensorOperator[currType()]->setEnabled(true);
    }
    else {
        if (prompt) {prompt->setMode("连续配置");}
        SensorOperator[currType()]->setEnabled(false);
    }
}

SensorConfigWidget::ConfigMode SensorConfigWidget::getConfigMode()
{
    return this->mode;
}

void SensorConfigWidget::selectSensorChange(const QString &text)
{
    if (configEditWidget.contains(currType())) {
        EventRecord::addEventObjValue(sensorTypeCombox, text);
        SetOption::setValue(iniMainGroup, "selectSensor", text);
        typeStacke->setCurrentWidget(configEditWidget[currType()]);
        SensorOperator[currType()]->setDefConfig(currType());
        presetProjectResh();
    }
}

void SensorConfigWidget::saveBtnSlot()
{
    setConfigMode(MANNUL);
    startConfig(SensorOperator[currType()]->getDevAddr());
    SensorOperator[currType()]->setDevAddr(SensorOperator[currType()]->getDevAddr());
}

void SensorConfigWidget::saveAsProject()
{
    QStringList prolist;

    parentDB->selectProjectList(currType(), prolist);
    savePreset->setProjectList(prolist);
    savePreset->exec();
    presetProjectResh();
}

void SensorConfigWidget::changeModeBtnSlot()
{
    if (getConfigMode() == MANNUL) {
        if (presetValBox->currentText() == presetBoxDefVal) {
            QMessageBox::warning(NULL, tr("模式选择"), "使用连续配置模式时需先选择配置方案", tr("是"));
            return ;
        }
        if (SensorOperator[currType()]->presetIsModify(presetValBox->currentText())) {
            QMessageBox::warning(NULL, tr("模式选择"), "配置方案已被修改请保存后再使用", tr("是"));
            return ;
        }
        setConfigMode(AUTO);
        prompt->setInfomation("连续配置模式中");
        prompt->showPrompt();
    }
    else {
        setConfigMode(MANNUL);
    }
}

void SensorConfigWidget::presetProjectChange(int index)
{
    if (presetValBox->currentText() == presetBoxDefVal) {
        SensorOperator[currType()]->setDefConfig(currType());
    }
    else {
        SensorOperator[currType()]->loadPreset(presetValBox->itemText(index));
    }
    EventRecord::addEventObjValue(presetValBox, presetValBox->currentText());
}

void SensorConfigWidget::startConfig(const QString &addr)
{
    QString err;

    if (!SensorOperator[currType()]->isValidConfig(&err)) {
        if (getConfigMode() == MANNUL) {
            QMessageBox::warning(NULL, "配置", err + "!", "是");
        }
        else if (getConfigMode() == AUTO) {
            prompt->setError(err);
        }
        return ;
    }

    SensorOperator[currType()]->apply();
    SensorOperator[currType()]->setDevAddr(addr);
    prompt->setDevAddr(addr);
    prompt->setInfomation("等待手动复位设备");
    prompt->setMovieSpeed(500);
    prompt->setSensorType(currType());
    prompt->showPrompt();
}

void SensorConfigWidget::stopConfig()
{
    SensorOperator[currType()]->cancelApply();
    prompt->setMovieSpeed(100);
}

void SensorConfigWidget::sensorWidgetMapInit()
{
    /*
     * 添加新的传感器支持时只需要在这里添加就可以了，
     * 页面的操作会统一调用SensorOperation类虚函数 ，
     * 新传感类继承SensorOperation类根据需要重写虚函数。
    */
    electricalFireSensor = new ElectricalFireSensor();
    electricalFireSensor->installEventRecord(SensorData::sensorCodeString(SensorData::EFS));
    owlhSensor = new OutdoorWLHS();
    owlhSensor->installEventRecord(SensorData::sensorCodeString(SensorData::OWDS) + \
                                   SensorData::sensorCodeString(SensorData::OWPS));
    temperatureSensor = new TemperatureSensor();
    temperatureSensor->installEventRecord(SensorData::sensorCodeString(SensorData::TES));
    humituresensor = new HumitureSensor();
    humituresensor->installEventRecord(SensorData::sensorCodeString(SensorData::THS));
    waterpresssensor = new WaterPressSensor();
    waterpresssensor->installEventRecord(SensorData::sensorCodeString(SensorData::WPS));
    waterlevelsensor = new WaterLevelSensor();
    waterlevelsensor->installEventRecord(SensorData::sensorCodeString(SensorData::WDS));
    switchvalsensor = new SwitchValSensor();
    switchvalsensor->installEventRecord(SensorData::sensorCodeString(SensorData::SNO));
    threephasevoltagesensor = new ThreephaseVoltageSensor();
    threephasevoltagesensor->installEventRecord(SensorData::sensorCodeString(SensorData::PVM));
    threephaseelectricity = new ThreephaseElectricity();
    threephaseelectricity->installEventRecord(SensorData::sensorCodeString(SensorData::PCM));
    residualelectricitysensor = new ResidualElectricitySensor();
    residualelectricitysensor->installEventRecord(SensorData::sensorCodeString(SensorData::PRC));
    threephaseswitchsensor = new ThreephaseSwitchSensor();
    threephaseswitchsensor->installEventRecord(SensorData::sensorCodeString(SensorData::PSD));
    waterpumpsensor = new WaterpumpSensor();
    waterpumpsensor->installEventRecord(SensorData::sensorCodeString(SensorData::NPWS));

    configEditWidget.insert(SensorData::TES, temperatureSensor);
    configEditWidget.insert(SensorData::THS, humituresensor);
    configEditWidget.insert(SensorData::WPS, waterpresssensor);
    configEditWidget.insert(SensorData::WDS, waterlevelsensor);
    configEditWidget.insert(SensorData::SNO, switchvalsensor);
    configEditWidget.insert(SensorData::PVM, threephasevoltagesensor);
    configEditWidget.insert(SensorData::PCM, threephaseelectricity);
    configEditWidget.insert(SensorData::PRC, residualelectricitysensor);
    configEditWidget.insert(SensorData::PSD, threephaseswitchsensor);
    configEditWidget.insert(SensorData::EFS, electricalFireSensor);
    configEditWidget.insert(SensorData::OWPS, owlhSensor);
    configEditWidget.insert(SensorData::OWDS, owlhSensor);
    configEditWidget.insert(SensorData::NPWS, waterpumpsensor);
    QMap<SensorData::sensorType, QWidget *>::const_iterator \
            i = configEditWidget.constBegin();
    while (i != configEditWidget.constEnd()) {
        typeStacke->addWidget(i.value());
        i.value()->setStyleSheet(".QWidget {border:2px solid #63B8FF;}");
        ++i;
    }

    SensorOperator.insert(SensorData::TES, temperatureSensor);
    SensorOperator.insert(SensorData::THS, humituresensor);
    SensorOperator.insert(SensorData::WPS, waterpresssensor);
    SensorOperator.insert(SensorData::WDS, waterlevelsensor);
    SensorOperator.insert(SensorData::SNO, switchvalsensor);
    SensorOperator.insert(SensorData::PVM, threephasevoltagesensor);
    SensorOperator.insert(SensorData::PCM, threephaseelectricity);
    SensorOperator.insert(SensorData::PRC, residualelectricitysensor);
    SensorOperator.insert(SensorData::PSD, threephaseswitchsensor);
    SensorOperator.insert(SensorData::EFS, electricalFireSensor);
    SensorOperator.insert(SensorData::OWPS, owlhSensor);
    SensorOperator.insert(SensorData::OWDS, owlhSensor);
    SensorOperator.insert(SensorData::NPWS, waterpumpsensor);
    QMap<SensorData::sensorType, SensorOperation *>::const_iterator\
            SO = SensorOperator.constBegin();
    while (SO != SensorOperator.constEnd()) {
        SO.value()->setDB(parentDB);
        ++SO;
    }
}

SensorData::sensorType SensorConfigWidget::currType()
{
    return SensorData::sensorStringCode(sensorTypeCombox->currentText());
}

void SensorConfigWidget::setini()
{
    sensorTypeCombox->setCurrentText(SetOption::value(iniMainGroup, "selectSensor").toString());
}

void SensorConfigWidget::eventRecordInit()
{
    EventRecord::addEventObj(sensorTypeCombox, nullptr, EventRecord::sensorSelectObjName);
    EventRecord::addEventObj(presetValBox, nullptr, EventRecord::presetSelectObjName);
}

void SensorConfigWidget::cannelConfig()
{
    prompt->setInfomation("配置已被取消!");
    setConfigMode(MANNUL);
    stopConfig();
}
