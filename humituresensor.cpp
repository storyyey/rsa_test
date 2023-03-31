#include "humituresensor.h"

HumitureSensor::HumitureSensor()
{

}

bool HumitureSensor::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::THS;

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

void HumitureSensor::setDefConfig(SensorData::sensorType sensorType)
{
    if (sensorType != SensorData::THS) {
        return ;
    }

    LowThresholdEdit(true);
    HighThresholdEdit(true);
    humidityEdit(true);
    variationEnableEdit(false);

    cmsConfig thsdef;

    thsdef.base.wm = SensorData::ABP;
    thsdef.base.addr = "50000001";
    thsdef.base.appskey = "42c60a4d148c719923b14a8f4d44da29";
    thsdef.base.nwskey = "42c60a4d148c719923b14a8f4d44da29";
    thsdef.base.channelNum = 10;

    thsdef.st = SensorData::THS;
    thsdef.lowtv = 16;
    thsdef.lowhumidity = 30;
    thsdef.lowtvUnit = "℃";
    thsdef.hightv = 27;
    thsdef.highhumidity = 90;
    thsdef.hightvUnit = "℃";
    thsdef.variation = 0;
    thsdef.variationUnit = "℃";

    thsdef.sampleTime = 60;
    thsdef.upTransTime = 7200;
    thsdef.reTrans = 2;

    setConfig(thsdef);
}

bool HumitureSensor::presetIsModify(const QString &name)
{
    return IsModify(SensorData::THS, name);
}

bool HumitureSensor::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::THS, name);
}

bool HumitureSensor::loadPreset(const QString &name)
{
    return load(SensorData::THS, name);
}
