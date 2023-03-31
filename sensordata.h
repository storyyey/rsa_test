#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>
#include <QWidget>
#include <QList>

class SensorData : public QObject
{
    Q_OBJECT
public:
    explicit SensorData(QObject *parent = nullptr); 
    enum sensorType {
        NON,
        TES, /* 温度 */
        THS, /* 温湿度 */
        WPS, /* 水压 */
        WDS, /* 水位 */
        SNO, /* 开关量 */
        PVM, /* 三相电压 */
        PCM, /* 三相电流 */
        PRC, /* 剩余电流 */
        PSD, /* 三相开关 */
        EFS, /* 电气火灾 */
        OWPS, /* 室外水压 */
        OWDS, /* 室外水位 */
        NPWS, /* 水泵 */
        STMAX,
    };

    enum workMode {
        ABP,
        OTAA,
    };

    class sensorBaseConfig {
        public:
            SensorData::workMode wm = ABP;
            QString addr; /* 设备地址 */
            QString appskey;
            QString nwskey;
            uint32_t channelNum = 0;
    };

    class configRecord {
        public:
            QString time;
            sensorType type;
            QString addr;
            QString cfile;
            bool state;
    };

    static QString sensorCodeString(enum sensorType st) {
        const char *ststring[STMAX] = {nullptr};

        if (st >= STMAX) {
            return QString("(%1)错误类型").arg(st);
        }

        ststring[TES] = "(TES)温度";
        ststring[THS] = "(THS)温湿度";
        ststring[WPS] = "(WPS)水压";
        ststring[WDS] = "(WDS)水位";
        ststring[SNO] = "(SNO)开关量";
        ststring[PVM] = "(PVM)三相电压";
        ststring[PCM] = "(PCM)三相电流";
        ststring[PRC] = "(PRC)剩余电流";
        ststring[PSD] = "(PSD)三相开关";
        ststring[EFS] = "(EFS)电气火灾";
        ststring[OWPS] = "(OWPS)室外水压";
        ststring[OWDS] = "(OWDS)室外水位";
        ststring[NPWS] = "(NPWS)水泵";

        return ststring[st] ? QString(ststring[st]) : QString("(%1)未知").arg(st);
    }

    static enum sensorType sensorStringCode(const QString &str) {
        QList<enum sensorType> codeList = {TES, THS, WPS, WDS, SNO, PVM, PCM, PRC, PSD, EFS, OWPS, OWDS, NPWS};

        for (int index = 0; index < codeList.count(); index++) {
            if (sensorCodeString(codeList[index]) == str) {
                return codeList[index];
            }
        }

        return NON;
    }

    static uint32_t switchType(enum sensorType st) {
        if (st == SNO || st == PRC || st == PSD) {
            return 1;
        }
        else {
            return 0;
        }
    }
    static uint32_t thresholdMp(enum sensorType st) {
        if (st == WPS || st == OWPS) {
            return 1000;
        }
        else if (st == SNO || st == PVM || st == PRC || st == PSD ) {
            return 1;
        }
        else {
            return 100;
        }
    }
signals:

};

class cmsConfig  {
public:
    SensorData::sensorType st = SensorData::NON;
    SensorData::sensorBaseConfig base;
    float lowtv = 0; /* 低阈值 */
    float lowhumidity = 0;
    QString lowtvUnit;
    float hightv = 0; /* 高阈值 */
    float highhumidity = 0;
    QString hightvUnit;
    float variation = 0; /* 变化量 */
    float variation2 = 0;
    QString variationUnit;

    uint32_t sampleTime = 0; /* 采样时间 */
    uint32_t upTransTime = 0; /* 上传时间 */
    uint32_t reTrans = 0; /* 重传次数 */
    bool operator==(cmsConfig &data) {
        if (this->base.wm != data.base.wm ||\
            this->base.appskey != data.base.appskey||\
            this->base.nwskey != data.base.nwskey||\
            this->base.channelNum != data.base.channelNum||\
            this->lowtv != data.lowtv ||\
            this->hightv != data.hightv ||\
            this->variation != data.variation ||\
            this->sampleTime != data.sampleTime ||\
            this->upTransTime != data.upTransTime ||\
            this->reTrans != data.reTrans) {

            return false;
        }

        return true;
    }
    bool operator!=(cmsConfig &data) {
        return !(*this == data);
    }
};

class efsConfig: public SensorData {
    public:
        sensorBaseConfig base;
        QString deveui;
        QString appeui;
        QString appkey;
        uint32_t hstime;
        uint32_t hsnum;
        uint32_t retrans;
        uint32_t tempth[4];
        uint32_t curth;
        uint32_t prcth;
        uint32_t ratio;
        uint32_t wrie;
        bool operator==(efsConfig &data) {
            if (this->base.wm != data.base.wm ||\
                this->base.appskey != data.base.appskey||\
                this->base.nwskey != data.base.nwskey||\
                this->base.channelNum != data.base.channelNum||\
                this->deveui != data.deveui ||\
                this->appeui != data.appeui ||\
                this->appkey != data.appkey ||\
                this->hstime != data.hstime ||\
                this->retrans != data.retrans ||\
                this->curth != data.curth ||\
                this->prcth != data.prcth ||\
                this->ratio != data.ratio ||\
                this->wrie != data.wrie ||\
                this->tempth[0] != data.tempth[0] ||\
                this->tempth[1] != data.tempth[1] ||\
                this->tempth[2] != data.tempth[2] ||\
                this->tempth[3] != data.tempth[3]) {

                return false;
            }

            return true;
        }

        bool operator!=(efsConfig &data) {
            return !(*this == data);
        }
};

class OWLHConfig: public SensorData {
    public:
        enum lcdDisplayMode {
            DISPLAYINT = 0,
            DISPLAYFLOAT = 1,
        };
        SensorData::sensorType st = SensorData::NON;
        sensorBaseConfig base;
        QString deveui;
        QString appeui;
        QString appkey;
        uint32_t sampletime = 0;
        uint32_t upintervalTime = 0;
        uint32_t retrans = 0;
        lcdDisplayMode displayMode = DISPLAYINT;
        float lowth = 0; /* 低阈值 */
        float highth = 0; /* 高阈值 */
        float variation = 0; /* 变化量 */
        bool operator==(OWLHConfig &data) {
            if (this->base.wm != data.base.wm ||\
                this->base.appskey != data.base.appskey||\
                this->base.nwskey != data.base.nwskey||\
                this->base.channelNum != data.base.channelNum||\
                this->deveui != data.deveui ||\
                this->appeui != data.appeui ||\
                this->appkey != data.appkey ||\
                this->upintervalTime != data.upintervalTime ||\
                this->retrans != data.retrans ||\
                this->lowth != data.lowth ||\
                this->highth != data.highth ||\
                this->sampletime != data.sampletime ||\
                this->variation != data.variation) {

                return false;
            }

            return true;
        }

        bool operator!=(OWLHConfig &data) {
            return !(*this == data);
        }
};
#endif // SENSORDATA_H
