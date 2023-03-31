#include "threephaseelectricity.h"

ThreephaseElectricity::ThreephaseElectricity()
{

}

bool ThreephaseElectricity::apply()
{
    cmsConfig cfg;

    getConfig(cfg);
    if (getATcmd() && !getATcmd()->isIdle()) {
        if (QMessageBox::warning(NULL, tr("配置"), QString("当前有配置未完成是否继续?"), tr("是"), tr("否"), 0, 1)) {
            return false;
        }
    }
    cancelApply();
    cfg.st = SensorData::PCM;

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

void ThreephaseElectricity::setDefConfig(SensorData::sensorType sensorType)
{
    if (sensorType != SensorData::PCM) {
        return ;
    }

    LowThresholdEdit(false);
    HighThresholdEdit(true);
    humidityEdit(false);
    variationEnableEdit(false);

    cmsConfig pcmdef;

    pcmdef.base.wm = SensorData::ABP;
    pcmdef.base.addr = "50000001";
    pcmdef.base.appskey = "46c60a4d148c719923b14a8f4d44da29";
    pcmdef.base.nwskey = "46c60a4d148c719923b14a8f4d44da29";
    pcmdef.base.channelNum = 10;

    pcmdef.st = SensorData::PCM;
    pcmdef.lowtv = 0;
    pcmdef.lowtvUnit = "A";
    pcmdef.hightv = 50;
    pcmdef.hightvUnit = "A";
    pcmdef.variation = 0;
    pcmdef.variationUnit = "A";

    pcmdef.sampleTime = 120;
    pcmdef.upTransTime = 14400;
    pcmdef.reTrans = 2;

    setConfig(pcmdef);
}

bool ThreephaseElectricity::presetIsModify(const QString &name)
{
    return IsModify(SensorData::PCM, name);
}

bool ThreephaseElectricity::saveAsPreset(const QString &name)
{
    return saveAs(SensorData::PCM, name);
}

bool ThreephaseElectricity::loadPreset(const QString &name)
{
    return load(SensorData::PCM, name);
}
