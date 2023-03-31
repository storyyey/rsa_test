#include "switchvalsensor.h"

SwitchValSensor::SwitchValSensor()
{

}

bool SwitchValSensor::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::SNO;

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

void SwitchValSensor::setDefConfig(SensorData::sensorType sensorType)
{
    if (sensorType != SensorData::SNO) {
        return ;
    }

    LowThresholdEdit(false);
    HighThresholdEdit(false);
    humidityEdit(false);
    variationEnableEdit(false);

    cmsConfig smodef;

    smodef.base.wm = SensorData::ABP;
    smodef.base.addr = "50000001";
    smodef.base.appskey = "49c60a4d148c719923b14a8f4d44da29";
    smodef.base.nwskey = "49c60a4d148c719923b14a8f4d44da29";
    smodef.base.channelNum = 10;

    smodef.st = SensorData::SNO;
    smodef.lowtv = 0;
    smodef.lowtvUnit = "";
    smodef.hightv = 0;
    smodef.hightvUnit = "";
    smodef.variation = 0;
    smodef.variationUnit = "";

    smodef.sampleTime = 60;
    smodef.upTransTime = 14400;
    smodef.reTrans = 2;

    setConfig(smodef);
}

bool SwitchValSensor::presetIsModify(const QString &name)
{
    return IsModify(SensorData::SNO, name);
}

bool SwitchValSensor::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::SNO, name);
}

bool SwitchValSensor::loadPreset(const QString &name)
{
    return load(SensorData::SNO, name);
}
