#include "temperaturesensor.h"

TemperatureSensor::TemperatureSensor()
{

}

bool TemperatureSensor::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::TES;

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

void TemperatureSensor::setDefConfig(SensorData::sensorType sensorType)
{
    if (sensorType != SensorData::TES) {
        return ;
    }

    LowThresholdEdit(true);
    HighThresholdEdit(true);
    humidityEdit(false);
    variationEnableEdit(false);

    cmsConfig tesdef;

    tesdef.base.wm = SensorData::ABP;
    tesdef.base.addr = "50000001";
    tesdef.base.appskey = "41c60a4d148c719923b14a8f4d44da29";
    tesdef.base.nwskey = "41c60a4d148c719923b14a8f4d44da29";
    tesdef.base.channelNum = 10;

    tesdef.st = SensorData::TES;
    tesdef.lowtv = 0;
    tesdef.lowtvUnit = "℃";
    tesdef.hightv = 70;
    tesdef.hightvUnit = "℃";
    tesdef.variation = 0;
    tesdef.variationUnit = "℃";

    tesdef.sampleTime = 60;
    tesdef.upTransTime = 14400;
    tesdef.reTrans = 2;

    setConfig(tesdef);
}

bool TemperatureSensor::presetIsModify(const QString &name)
{
    return IsModify(SensorData::TES, name);
}

bool TemperatureSensor::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::TES, name);
}

bool TemperatureSensor::loadPreset(const QString &name)
{
    return load(SensorData::TES, name);
}
