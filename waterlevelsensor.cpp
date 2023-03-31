#include "waterlevelsensor.h"

WaterLevelSensor::WaterLevelSensor()
{
    this->setObjectName("水位");
}

bool WaterLevelSensor::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::WDS;

    getATcmd()->addCmd(getATcmd()->enterATModeCmd(), 1000, 20);
    getATcmd()->addCmd(getATcmd()->setWorkModeCmd(cfg.base.wm));
    getATcmd()->addCmd(getATcmd()->setAddrCmd(cfg.base.addr));
    getATcmd()->addCmd(getATcmd()->setAppskeyCmd(cfg.base.appskey));
    getATcmd()->addCmd(getATcmd()->setNewskeyCmd(cfg.base.nwskey));
    getATcmd()->addCmd(getATcmd()->setChannel(cfg.base.channelNum));
    getATcmd()->addCmd(getATcmd()->setSensorCmd(cfg.st, SensorData::switchType(cfg.st), \
                                cfg.lowtv * SensorData::thresholdMp(cfg.st), \
                                cfg.lowhumidity * SensorData::thresholdMp(cfg.st),\
                                cfg.hightv * SensorData::thresholdMp(cfg.st),\
                                cfg.highhumidity * SensorData::thresholdMp(cfg.st),\
                                cfg.variation * SensorData::thresholdMp(cfg.st),\
                                cfg.variation2 * SensorData::thresholdMp(cfg.st)));
    getATcmd()->addCmd(getATcmd()->setSmodeCmd(cfg.sampleTime, cfg.upTransTime, cfg.reTrans));
    getATcmd()->addCmd(getATcmd()->saveCmd(), 8000);
    getATcmd()->addCmd(getATcmd()->exitATModeCmd());
    getATcmd()->addFinishFlag("Exit AT command mode now");

    getATcmd()->exec(QStringList() << "Send AT+CMD=1" << "into AT command mode");

    return true;
}

void WaterLevelSensor::setDefConfig(SensorData::sensorType sensorType)
{
    cmsConfig wdsdef;

    if (sensorType != SensorData::WDS) {
        return ;
    }

    LowThresholdEdit(true);
    HighThresholdEdit(true);
    humidityEdit(false);
    variationEnableEdit(true);

    wdsdef.base.wm = SensorData::ABP;
    wdsdef.base.addr = "50000001";
    wdsdef.base.appskey = "44c60a4d148c719923b14a8f4d44da29";
    wdsdef.base.nwskey = "44c60a4d148c719923b14a8f4d44da29";
    wdsdef.base.channelNum = 10;

    wdsdef.st = SensorData::WDS;
    wdsdef.lowtv = 0.10;
    wdsdef.lowtvUnit = "米";
    wdsdef.hightv = 5.0;
    wdsdef.hightvUnit = "米";
    wdsdef.variation = 0;
    wdsdef.variationUnit = "米";

    wdsdef.sampleTime = 60;
    wdsdef.upTransTime = 7200;
    wdsdef.reTrans = 2;

    setConfig(wdsdef);
}

bool WaterLevelSensor::presetIsModify(const QString &name)
{
    return IsModify(SensorData::WDS, name);
}

bool WaterLevelSensor::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::WDS, name);
}

bool WaterLevelSensor::loadPreset(const QString &name)
{
    return load(SensorData::WDS, name);
}
