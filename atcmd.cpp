#include "atcmd.h"
#include <QDebug>
#include "mainwindow.h"

ATCmd::ATCmd(QObject *parent) : QObject(parent)
{
    totimer = new QTimer(this);
    connect(totimer, SIGNAL(timeout()), this, SLOT(execTimeout()));
}

ATCmd::~ATCmd()
{
    qDebug() << "~ATCmd()";
}

bool ATCmd::isIdle()
{
    return execQueue.isEmpty();
}

QString ATCmd::enterATModeCmd()
{
    return CMDhead + cmdKey[CMD_ENTER] + QString("=%1").arg(1) + CMDtail;
}

QString ATCmd::getCmdSupportCmd()
{
    return CMDhead + cmdKey[CMD_GET_ALL] + CMDtail;
}

QString ATCmd::getVersionInfoCmd()
{
    return "AT" + cmdKey[CMD_VERSION] + "?" + CMDtail;
}

QString ATCmd::getWorkModeCmd()
{
    return "AT" + cmdKey[CMD_WORK_MODE] + "?" + CMDtail;
}

QString ATCmd::setWorkModeCmd(SensorData::workMode mode)
{
    return "AT" + cmdKey[CMD_WORK_MODE] + QString("=%1").arg(mode) + CMDtail;
}

QString ATCmd::getActiveModeCmd()
{
    return "AT" + cmdKey[CMD_ACTIVE_MODE] + "?" + CMDtail;
}

QString ATCmd::setActiveModeCmd(SensorData::workMode mode)
{
    return "AT" + cmdKey[CMD_ACTIVE_MODE] + QString("=%1").arg(mode) + CMDtail;
}

QString ATCmd::getAddrCmd()
{
    return "AT" + cmdKey[CMD_ADDR] + "?" + CMDtail;
}

QString ATCmd::setAddrCmd(const QString &addr)
{
    return "AT" + cmdKey[CMD_ADDR] + QString("=%1").arg(addr) + CMDtail;
}

QString ATCmd::getDevAddrCmd()
{
    return "AT" + cmdKey[CMD_DEVADDR] + "?" + CMDtail;
}

QString ATCmd::setDevaddrCmd(const QString &addr)
{
    return "AT" + cmdKey[CMD_DEVADDR] + QString("=%1").arg(addr) + CMDtail;
}

QString ATCmd::getAppskeyCmd()
{
    return "AT" + cmdKey[CMD_APPSKEY] + "?" + CMDtail;
}

QString ATCmd::setAppskeyCmd(const QString &key)
{
    return "AT" + cmdKey[CMD_APPSKEY] + QString("=%1").arg(key) + CMDtail;
}

QString ATCmd::getNewskeyCmd()
{
    return "AT" + cmdKey[CMD_NEWSKEY] + "?" + CMDtail;
}

QString ATCmd::setNewskeyCmd(const QString &key)
{
    return "AT" + cmdKey[CMD_NEWSKEY] + QString("=%1").arg(key) + CMDtail;
}

QString ATCmd::getAppeuiCmd()
{
    return "AT" + cmdKey[CMD_APPEUI] + "?" + CMDtail;
}

QString ATCmd::setAppeuiCmd(const QString &eui)
{
    return "AT" + cmdKey[CMD_APPEUI] + QString("=%1").arg(eui) + CMDtail;
}

QString ATCmd::getDeveuiCmd()
{
    return "AT" + cmdKey[CMD_DEVEUI] + "?" + CMDtail;
}

QString ATCmd::setDeveuiCmd(const QString &eui)
{
    return "AT" + cmdKey[CMD_DEVEUI] + QString("=%1").arg(eui) + CMDtail;
}

QString ATCmd::getAppkeyCmd()
{
    return "AT" + cmdKey[CMD_APPKEY] + "?" + CMDtail;
}

QString ATCmd::setAppkeyCmd(const QString &key)
{
    return "AT" + cmdKey[CMD_APPKEY] + QString("=%1").arg(key) + CMDtail;
}

QString ATCmd::getSmodeCmd()
{
    return "AT" + cmdKey[CMD_SMODE] + "?" + CMDtail;
}

QString ATCmd::setSmodeCmd(uint32_t sampltTime, uint32_t upTransTime, uint32_t retrans)
{
    return "AT" + cmdKey[CMD_SMODE] + QString("=%1,%2,%3").arg(sampltTime).arg(upTransTime).arg(retrans) + CMDtail;
}

QString ATCmd::getSensorCmd()
{
    return "AT" + cmdKey[CMD_SENSOR] + "?" + CMDtail;
}

QString ATCmd::setSensorCmd(SensorData::sensorType type, uint32_t switchType, float lowth1, float lowth2, float highth1, float highth2, float var1, float var2)
{
    return "AT" + cmdKey[CMD_SENSOR] + QString("=%1,%2,%3:%4,%5:%6,%7:%8")\
            .arg(type).arg(switchType).arg(lowth1).arg(lowth2)\
            .arg(highth1).arg(highth2).arg(var1).arg(var2)+ CMDtail;
}

QString ATCmd::getChannel()
{
    return "AT" + cmdKey[CMD_CHANNEL] + "?" + CMDtail;
}

QString ATCmd::setChannel(uint32_t channel)
{
    return "AT" + cmdKey[CMD_CHANNEL] +QString("=%1").arg(channel) + CMDtail;
}

QString ATCmd::getModeCmd()
{
    return "AT" + cmdKey[CMD_MODE] + "?" + CMDtail;
}

QString ATCmd::setModeCmd(uint32_t shtime, uint32_t retrans)
{
    return "AT" + cmdKey[CMD_MODE] +QString("=%1,%2").arg(shtime).arg(retrans) + CMDtail;
}

QString ATCmd::getTemtresholdCmd()
{
    return "AT" + cmdKey[CMD_TEMTRESHOLD] + "?" + CMDtail;
}

QString ATCmd::setTemtresholdCmd(float tem1, float tem2, float tem3, float tem4)
{
    return "AT" + cmdKey[CMD_TEMTRESHOLD] + QString("=%1,%2,%3,%4")\
            .arg(tem1).arg(tem2).arg(tem3).arg(tem4) + CMDtail;
}

QString ATCmd::getCurtresholdCmd()
{
    return "AT" + cmdKey[CMD_CURTRESHOLD] + "?" + CMDtail;
}

QString ATCmd::setCurtresholdCmd(float cur)
{
     return "AT" + cmdKey[CMD_CURTRESHOLD] +QString("=%1").arg(cur) + CMDtail;
}

QString ATCmd::getPrctresholdCmd()
{
    return "AT" + cmdKey[CMD_PRCTRESHOLD] + "?" + CMDtail;
}

QString ATCmd::setPrctresholdCmd(float prc)
{
    return "AT" + cmdKey[CMD_PRCTRESHOLD] +QString("=%1").arg(prc) + CMDtail;
}

QString ATCmd::getRatioCmd()
{
    return "AT" + cmdKey[CMD_RATIO] + "?" + CMDtail;
}

QString ATCmd::setRatioCmd(float ratio)
{
    return "AT" + cmdKey[CMD_RATIO] +QString("=%1").arg(ratio) + CMDtail;
}

QString ATCmd::getWireCmd()
{
    return "AT" + cmdKey[CMD_WIRE] + "?" + CMDtail;
}

QString ATCmd::setWireCmd(uint32_t wire)
{
    return "AT" + cmdKey[CMD_WIRE] + QString("=%1").arg(wire) + CMDtail;
}

QString ATCmd::saveCmd()
{
    return "AT" + cmdKey[CMD_SAVE] + QString("=%1").arg(255) + CMDtail;
}

QString ATCmd::exitATModeCmd()
{
    return "AT" + cmdKey[CMD_EXIT] + QString("=%1").arg(0) + CMDtail;
}

QString ATCmd::setVpgain()
{
    return "AT" + cmdKey[CMD_VPGAIN] + "=1" + CMDtail;
}

QString ATCmd::setCpgain()
{
    return "AT" + cmdKey[CMD_CPGAIN] + "=1" + CMDtail;
}

QString ATCmd::setCNgain()
{
    return "AT" + cmdKey[CMD_CNGAIN] + "=1" + CMDtail;
}

QString ATCmd::setCpoffset()
{
    return "AT" + cmdKey[CMD_CPOFFSET] + "=1" + CMDtail;
}

QString ATCmd::setCnoffset()
{
    return "AT" + cmdKey[CMD_CNOFFSET] + "=1" + CMDtail;
}

QString ATCmd::getWorkParame()
{
    return "AT" + cmdKey[CMD_WORK_PARAME] + "?" + CMDtail;
}

QString ATCmd::setWorkParame(uint32_t upinter, uint32_t retrans, uint32_t sampleTime)
{
    if (sampleTime) {
        return "AT" + cmdKey[CMD_WORK_PARAME] +QString("=%1,%2,%3").arg(sampleTime).arg(upinter).arg(retrans) + CMDtail;
    }
    else {
        return "AT" + cmdKey[CMD_WORK_PARAME] +QString("=%1,%2").arg(upinter).arg(retrans) + CMDtail;
    }
}

QString ATCmd::getLcdDisplayMode()
{
    return "AT" + cmdKey[CMD_LCD_DISPLAY_MODE] + "?" + CMDtail;
}

QString ATCmd::setLcdDisplayMode(OWLHConfig::lcdDisplayMode mode)
{
    return "AT" + cmdKey[CMD_LCD_DISPLAY_MODE] +QString("=%1").arg(mode) + CMDtail;
}

QString ATCmd::getTreshord()
{
    return "AT" + cmdKey[CMD_TRESHOLD]  + "?" + CMDtail;
}

QString ATCmd::setTreshord(float lowth, float highth, float var)
{
    return "AT" + cmdKey[CMD_TRESHOLD] +QString("=%1,%2,%3").arg(lowth).arg(highth).arg(var) + CMDtail;
}

bool ATCmd::addCmd(const QString &cmd, uint32_t timeout, uint32_t rt)
{
    if (!cmd.size()) {
        windowConsole("添加命令失败,命令长度不能为0");
        return false;
    }

    execCmd ec;
    ec.cmd = cmd;
    ec.timeout = timeout;
    ec.retrans = rt;

    execQueue.append(ec);
    windowConsole("添加待发送命令:" + cmd);

    return true;
}

bool ATCmd::exec()
{
    if (!execQueue.isEmpty()) {
        totimer->setInterval(execQueue.head().timeout);
        totimer->start();
        emit next(execQueue.head().cmd);
        return true;
    }
    else {
        return false;
    }
}

bool ATCmd::exec(const QStringList &key)
{
    this->execFlag = key;

    return true;
}

bool ATCmd::isExecTrigger(const QString &cond)
{
    if (execFlag.size()) {
        for (int n = 0; n < execFlag.size(); n++) {
            if (cond.contains(execFlag.at(n))) {
                execFlag.clear();
                return exec();
            }
        }
    }

    return false;
}

bool ATCmd::parserResponse(const QString &resopn)
{
    if (execQueue.isEmpty() || !resopn.size()) {
        return false;
    }
    if (execFlag.size()) {
        return false;
    }

    execCmd &ec = execQueue.head();
    this->respon += resopn;

    //qDebug() << respon << ">>>>" << ec.cmd.split("=").at(0).mid(2) << "<<<<<";
    //if (this->respon.contains(ec.cmd.split("=").at(0).mid(2))) {

    if (finishFlag.size() && \
        this->respon.contains(finishFlag)) {
        windowConsole(QString("发现命令发送结束标志(%1)").arg(finishFlag));
        this->respon.clear();
        emit finish(SUCCESS);
        clear();
    }
    else if (this->respon.contains(succesKey)) {
        windowConsole(QString("发现命令发送成功标志(%1),命令[%2]发送结束").arg(succesKey, ec.cmd));
        this->respon.clear();
        execQueue.takeFirst();
        if (exec() == false) {
            emit finish(SUCCESS);
            clear();
        }
    }
    else if (this->respon.contains(errorKey)) {
        windowConsole(QString("发现命令发送失败标志(%1),命令[%2]发送结束").arg(errorKey, ec.cmd));
        this->respon.clear();
        errorCmd = ec.cmd;
        emit finish(ERROR);
        clear();
    }
    else if (this->respon.size() > 512) {
        this->respon.clear();
    }
    else {

    }

    return true;
}

void ATCmd::clear()
{
    windowConsole("清除队列中等待发送的命令");
    totimer->stop();
    execFlag.clear();
    execQueue.clear();
    finishFlag.clear();
    respon.clear();
}

void ATCmd::addFinishFlag(const QString &flag)
{
    this->finishFlag = flag;
}

QStringList ATCmd::cmdDump()
{
    QStringList sl;

    if (execQueue.isEmpty()) {
        windowConsole("当前无需发送的命令!");
        return sl;
    }
    for (int index = 0; index < execQueue.count(); index++) {
        sl << execQueue.at(index).cmd;
        windowConsole(QString("待发送命令[%1]:%2").arg(index).arg(execQueue.at(index).cmd));
    }

    return sl;
}

QString ATCmd::getError()
{
    return errorCmd;
}

QStringList ATCmd::getCmdLst()
{
    QStringList sl;

    QMap<enum cmdType, QString>::iterator iter = cmdKey.begin();
    while (iter != cmdKey.end()) {
        sl << "AT" + iter.value();
        iter++;
    }

    return sl;
}

QString ATCmd::description(const QString &cmd)
{
    QString key = cmd.mid(cmd.indexOf("+"), cmd.indexOf(QRegExp("[\\?\\=]")) - 2);
    if (cmddesc.contains(key)) {
        return cmddesc[key];
    }
    return "";
}

void ATCmd::execTimeout()
{
    if (execQueue.isEmpty()) {
        return ;
    }
    cmdDump();
    execCmd &ec = execQueue.head();
    totimer->stop();

    windowConsole("发送命令超时:" + ec.cmd);
    if (ec.retrans > 0) {
        windowConsole("重新发送命令:" + ec.cmd);
        exec();
        ec.retrans--;
    }
    else {
        windowConsole("命令发送失败结束所有命令发送:" + ec.cmd);
        errorCmd = ec.cmd;
        emit finish(ERROR);
        clear();
    }
}
