#ifndef ATCMD_H
#define ATCMD_H

#include <QObject>
#include <QMap>
#include <QQueue>
#include <QTimer>
#include "sensordata.h"

class ATCmd : public QObject
{
    Q_OBJECT
public:
    enum finishState {
        SUCCESS,
        ERROR,
    };

    enum cmdType {
        CMD_GET_ALL,
        CMD_ENTER,
        CMD_VERSION,
        CMD_WORK_MODE,
        CMD_ACTIVE_MODE,
        CMD_ADDR,
        CMD_DEVADDR,
        CMD_APPSKEY,
        CMD_NEWSKEY,
        CMD_APPEUI,
        CMD_DEVEUI,
        CMD_APPKEY,
        CMD_SMODE,
        CMD_SENSOR,
        CMD_CONF,
        CMD_CHANNEL,
        CMD_MODE,
        CMD_TEMTRESHOLD,
        CMD_CURTRESHOLD,
        CMD_PRCTRESHOLD,
        CMD_RATIO,
        CMD_WIRE,
        CMD_VPGAIN,
        CMD_CPGAIN,
        CMD_CNGAIN,
        CMD_CPOFFSET,
        CMD_CNOFFSET,
        CMD_WORK_PARAME,
        CMD_LCD_DISPLAY_MODE,
        CMD_TRESHOLD,
        CMD_SAVE,
        CMD_EXIT,
    };

    struct execCmd {
        QString cmd;
        uint32_t timeout;
        uint32_t retrans;
    };

    explicit ATCmd(QObject *parent = nullptr);
    ~ATCmd();
    bool isIdle();

    QString enterATModeCmd();
    QString getCmdSupportCmd();
    QString getVersionInfoCmd();
    QString getWorkModeCmd();
    QString setWorkModeCmd(SensorData::workMode mode);
    QString getActiveModeCmd();
    QString setActiveModeCmd(SensorData::workMode mode);
    QString getAddrCmd();
    QString setAddrCmd(const QString &addr);
    QString getDevAddrCmd();
    QString setDevaddrCmd(const QString &addr);
    QString getAppskeyCmd();
    QString setAppskeyCmd(const QString &key);
    QString getNewskeyCmd();
    QString setNewskeyCmd(const QString &key);
    QString getAppeuiCmd();
    QString setAppeuiCmd(const QString &eui);
    QString getDeveuiCmd();
    QString setDeveuiCmd(const QString &eui);
    QString getAppkeyCmd();
    QString setAppkeyCmd(const QString &key);
    QString getSmodeCmd();
    QString setSmodeCmd(uint32_t sampltTime, uint32_t upTransTime, uint32_t retrans);
    QString getSensorCmd();
    QString setSensorCmd(SensorData::sensorType type, uint32_t switchType, \
                         float lowth1, float lowth2,\
                         float highth1, float highth2,\
                         float var1, float var2);
    QString getChannel();
    QString setChannel(uint32_t channel);
    QString getModeCmd();
    QString setModeCmd(uint32_t shtime,  uint32_t retrans);
    QString getTemtresholdCmd();
    QString setTemtresholdCmd(float tem1, float tem2, float tem3, float tem4 );
    QString getCurtresholdCmd();
    QString setCurtresholdCmd(float cur);
    QString getPrctresholdCmd();
    QString setPrctresholdCmd(float prc);
    QString getRatioCmd();
    QString setRatioCmd(float ratio);
    QString getWireCmd();
    QString setWireCmd(uint32_t wire);
    QString saveCmd();
    QString exitATModeCmd();
    QString setVpgain();
    QString setCpgain();
    QString setCNgain();
    QString setCpoffset();
    QString setCnoffset();
    QString getWorkParame();
    QString setWorkParame(uint32_t upinter, uint32_t retrans, uint32_t sampleTime = 0);
    QString getLcdDisplayMode();
    QString setLcdDisplayMode(OWLHConfig::lcdDisplayMode mode);
    QString getTreshord();
    QString setTreshord(float lowth, float highth, float var);

    bool addCmd(const QString &cmd, uint32_t timeout = 4000, uint32_t rt  = 2);
    bool exec();
    bool exec(const QStringList &key);
    bool isExecTrigger(const QString &cond);
    bool parserResponse(const QString &resopn);
    void clear();
    void addFinishFlag(const QString &flag);
    QStringList cmdDump();
    QString getError();

    QStringList getCmdLst();

    QString description(const QString &cmd);
private:
    QMap<QString, QString> cmddesc =  {{"+?", "支持的AT命令"},\
                                      {"+CMD", "(1:进入AT模式,0:退出AT模式)"},\
                                      {"+GMI", "版本信息"},\
                                      {"+MODE", "工作模式(0:ABP,1:OTAA)(旧版本传感器支持)|上传数据间隔,重传次数(新版本传感器支持)|采样间隔,上传数据间隔,重传次数(新版本传感器支持)"},\
                                      {"+ACTIVEMODE", "激活模式(0:ABP,1:OTAA)"},\
                                      {"+ADDR", "设备地址(旧版本传感器支持)"},\
                                      {"+DEVADDR", "设备地址"},\
                                      {"+APPSKEY", "APPSKEY"},\
                                      {"+NEWSKEY", "NEWSKEY"},\
                                      {"+APPEUI", "APPEUI"},\
                                      {"+DEVEUI", "DEVEUI"},\
                                      {"+APPKEY", "APPKEY"},\
                                      {"+SMODE", "采样时间,上报时间,重传次数(旧版本传感器支持)"},\
                                      {"+SENSOR", "传感器类型,开关类型,低阈值,低阈值,高阈值:高阈值,变化阈值:变化阈值(旧版本传感器支持)"},\
                                      {"+CONF", "获取所有配置"},\
                                      {"+CHANNEL", "频段"},\
                                      {"+TEMTRESHOLD", "四路温度阈值"},\
                                      {"+CURTRESHOLD", "电流阈值"},\
                                      {"+PRCTRESHOLD", "剩余电流阈值"},\
                                      {"+RATIO", "电流互感器变比"},\
                                      {"+WIRE", "传感器线制"},\
                                      {"+VPGAIN", "三相电压校准"},\
                                      {"+CPGAIN", "三相电流校准"},\
                                      {"+CNGAIN", "剩余电流校准"},\
                                      {"+CPOFFSET", "三相电流校零"},\
                                      {"+CNOFFSET", "剩余电流校零"},\
                                      {"+DISPLAYMODE", "LCD显示模式(0:整数,1小数)"},\
                                      {"+TRESHOLD", "阈值(低阈值,高阈值,变化阈值)"},\
                                      {"+INI", "保存配置"}};

    QMap<enum cmdType, QString> cmdKey = {{CMD_GET_ALL, "+?"},\
                                          {CMD_ENTER, "+CMD"},\
                                          {CMD_VERSION, "+GMI"},\
                                          {CMD_WORK_MODE, "+MODE"},\
                                          {CMD_ACTIVE_MODE, "+ACTIVEMODE"},\
                                          {CMD_ADDR, "+ADDR"},\
                                          {CMD_DEVADDR, "+DEVADDR"},\
                                          {CMD_APPSKEY, "+APPSKEY"},\
                                          {CMD_NEWSKEY, "+NEWSKEY"},\
                                          {CMD_APPEUI, "+APPEUI"},\
                                          {CMD_DEVEUI, "+DEVEUI"},\
                                          {CMD_APPKEY, "+APPKEY"},\
                                          {CMD_SMODE, "+SMODE"},\
                                          {CMD_SENSOR, "+SENSOR"},\
                                          {CMD_CONF, "+CONF"},\
                                          {CMD_CHANNEL, "+CHANNEL"},\
                                          {CMD_MODE, "+MODE"},\
                                          {CMD_TEMTRESHOLD, "+TEMTRESHOLD"},\
                                          {CMD_CURTRESHOLD, "+CURTRESHOLD"},\
                                          {CMD_PRCTRESHOLD, "+PRCTRESHOLD"},\
                                          {CMD_RATIO, "+RATIO"},\
                                          {CMD_WIRE, "+WIRE"},\
                                          {CMD_VPGAIN, "+VPGAIN"},\
                                          {CMD_CPGAIN, "+CPGAIN"},\
                                          {CMD_CNGAIN, "+CNGAIN"},\
                                          {CMD_CPOFFSET, "+CPOFFSET"},\
                                          {CMD_CNOFFSET, "+CNOFFSET"},\
                                          {CMD_WORK_PARAME, "+MODE"},\
                                          {CMD_LCD_DISPLAY_MODE, "+DISPLAYMODE"},\
                                          {CMD_TRESHOLD, "+TRESHOLD"},\
                                          {CMD_SAVE, "+INI"},\
                                          {CMD_EXIT, "+CMD"}};

    QQueue<execCmd> execQueue;
    QTimer *totimer;
    QString CMDhead = "AT";
    QString CMDtail = "\r\n";
    QString succesKey = "OK";
    QString errorKey = "ERROR";
    QStringList execFlag;
    QString respon;
    QString finishFlag;
    QString errorCmd;
signals:
    void finish(ATCmd::finishState state);
    void next(const QString &text);
private slots:
    void execTimeout();
};

#endif // ATCMD_H
