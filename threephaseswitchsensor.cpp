#include "threephaseswitchsensor.h"

ThreephaseSwitchSensor::ThreephaseSwitchSensor()
{

}

bool ThreephaseSwitchSensor::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::PSD;

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

void ThreephaseSwitchSensor::setDefConfig(SensorData::sensorType sensorType)
{
    if (sensorType != SensorData::PSD) {
        return ;
    }

    LowThresholdEdit(false);
    HighThresholdEdit(false);
    humidityEdit(false);
    variationEnableEdit(false);

    cmsConfig psddef;

    psddef.base.wm = SensorData::ABP;
    psddef.base.addr = "50000001";
    psddef.base.appskey = "4ac60a4d148c719923b14a8f4d44da29";
    psddef.base.nwskey = "4ac60a4d148c719923b14a8f4d44da29";
    psddef.base.channelNum = 10;

    psddef.st = SensorData::PSD;
    psddef.lowtv = 0;
    psddef.lowtvUnit = "";
    psddef.hightv = 0;
    psddef.hightvUnit = "";
    psddef.variation = 0;
    psddef.variationUnit = "";

    psddef.sampleTime = 0;
    psddef.upTransTime = 14400;
    psddef.reTrans = 2;

    setConfig(psddef);
}

bool ThreephaseSwitchSensor::presetIsModify(const QString &name)
{
    return IsModify(SensorData::PSD, name);
}

bool ThreephaseSwitchSensor::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::PSD, name);
}

bool ThreephaseSwitchSensor::loadPreset(const QString &name)
{
    return load(SensorData::PSD, name);
}
