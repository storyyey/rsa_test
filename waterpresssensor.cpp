#include "waterpresssensor.h"

WaterPressSensor::WaterPressSensor()
{

}

bool WaterPressSensor::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::WPS;

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

void WaterPressSensor::setDefConfig(SensorData::sensorType sensorType)
{
    if (sensorType != SensorData::WPS) {
        return ;
    }

    LowThresholdEdit(true);
    HighThresholdEdit(true);
    humidityEdit(false);
    variationEnableEdit(true);

    cmsConfig wpsdef;

    wpsdef.base.wm = SensorData::ABP;
    wpsdef.base.addr = "50000001";
    wpsdef.base.appskey = "43c60a4d148c719923b14a8f4d44da29";
    wpsdef.base.nwskey = "43c60a4d148c719923b14a8f4d44da29";
    wpsdef.base.channelNum = 10;

    wpsdef.st = SensorData::WPS;
    wpsdef.lowtv = 0.10;
    wpsdef.lowtvUnit = "MPa";
    wpsdef.hightv = 1.20;
    wpsdef.hightvUnit = "MPa";
    wpsdef.variation = 0;
    wpsdef.variationUnit = "MPa";

    wpsdef.sampleTime = 60;
    wpsdef.upTransTime = 7200;
    wpsdef.reTrans = 2;

    setConfig(wpsdef);
}

bool WaterPressSensor::presetIsModify(const QString &name)
{
    return IsModify(SensorData::WPS, name);
}

bool WaterPressSensor::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::WPS, name);
}

bool WaterPressSensor::loadPreset(const QString &name)
{
    return load(SensorData::WPS, name);
}
