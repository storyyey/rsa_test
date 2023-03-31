#include "threephasevoltagesensor.h"

ThreephaseVoltageSensor::ThreephaseVoltageSensor()
{

}

bool ThreephaseVoltageSensor::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::PVM;

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

void ThreephaseVoltageSensor::setDefConfig(SensorData::sensorType sensorType)
{
    if (sensorType != SensorData::PVM) {
        return ;
    }

    LowThresholdEdit(false);
    HighThresholdEdit(true);
    humidityEdit(false);
    variationEnableEdit(false);

    cmsConfig pvmdef;

    pvmdef.base.wm = SensorData::ABP;
    pvmdef.base.addr = "50000001";
    pvmdef.base.appskey = "45c60a4d148c719923b14a8f4d44da29";
    pvmdef.base.nwskey = "45c60a4d148c719923b14a8f4d44da29";
    pvmdef.base.channelNum = 10;

    pvmdef.st = SensorData::PVM;
    pvmdef.lowtv = 176;
    pvmdef.lowtvUnit = "V";
    pvmdef.hightv = 264;
    pvmdef.hightvUnit = "V";
    pvmdef.variation = 0;
    pvmdef.variationUnit = "V";

    pvmdef.sampleTime = 120;
    pvmdef.upTransTime = 14400;
    pvmdef.reTrans = 2;

    setConfig(pvmdef);
}

bool ThreephaseVoltageSensor::presetIsModify(const QString &name)
{
    return IsModify(SensorData::PVM, name);
}

bool ThreephaseVoltageSensor::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::PVM, name);
}

bool ThreephaseVoltageSensor::loadPreset(const QString &name)
{
    return load(SensorData::PVM, name);
}
