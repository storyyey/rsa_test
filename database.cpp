#include "database.h"
#include <QApplication>
#include "mainwindow.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    QString path = QDir::currentPath();
    QApplication::addLibraryPath(path + QString("/plugins"));
    QPluginLoader loader(path + QString("/plugins/sqldrivers/qsqlite.dll"));
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(path + "/sensor.db");


    if (!database.open()) {
       windowConsole("打开数据库失败" + QString(database.lastError().text()));
    }
    else {
        windowConsole("打开数据库成功");
        sqlquery = QSqlQuery(database);
    }

    createTableEfs();
    createTableCms();
    createTableOwlh();
    createTableConfigRecord();

    windowConsole(database.lastError().text());
}

DataBase::~DataBase()
{
    qDebug() << "~DataBase()";

    QString name;
    {
       name = QSqlDatabase::database().connectionName();
    }
    qDebug() << "connectionName:" <<name;
    sqlquery.clear();
    database.removeDatabase(name);
    database.close();
}

bool DataBase::isTableExist(const QString &name)
{
    sqlquery.exec(findTableSql.arg(name));

    return sqlquery.next();
}

bool DataBase::createTableEfs()
{
    if (!isTableExist(efsCfg)) {
        sqlquery.exec(QString("CREATE TABLE %1(\
                       NAME      TEXT NOT NULL,\
                       WORKMODE  INTEGER NOT NULL,\
                       APPSKEY   TEXT NOT NULL,\
                       NEWSKEY   TEXT NOT NULL,\
                       CHANNEL   INTEGER NOT NULL,\
                       DEVEUI    TEXT NOT NULL,\
                       APPEUI    TEXT NOT NULL,\
                       APPKEY    TEXT NOT NULL,\
                       HSTIME    INTEGER NOT NULL,\
                       HSNUM     INTEGER NOT NULL,\
                       RETRANS   INTEGER NOT NULL,\
                       TEMP      INTEGER NOT NULL,\
                       CURTH     INTEGER NOT NULL,\
                       PRCTH     INTEGER NOT NULL,\
                       RATIO     INTEGER NOT NULL,\
                       WRIE      INTEGER NOT NULL,\
                       STATE     INTEGER NOT NULL);").arg(efsCfg));
        windowConsole("创建表" + efsCfg);
    }

    return true;
}

bool DataBase::createTableCms()
{
    if (!isTableExist(cmsCfg)) {
        sqlquery.exec(QString("CREATE TABLE %1(\
                       NAME      TEXT NOT NULL,\
                       WORKMODE  INTEGER NOT NULL,\
                       APPSKEY   TEXT NOT NULL,\
                       NEWSKEY   TEXT NOT NULL,\
                       CHANNEL   INTEGER NOT NULL,\
                       LOWVPT    FLOAT NOT NULL,\
                       HIGHTVPT  FLOAT NOT NULL,\
                       VAR       FLOAT NOT NULL,\
                       LOWHUMIDITY      FLOAT,\
                       HIGHTHUMIDITY    FLOAT,\
                       SAMPLE       INTEGER NOT NULL,\
                       UPINTERVAL   INTEGER NOT NULL,\
                       RETRANS      INTEGER NOT NULL,\
                       TYPE         INTEGER NOT NULL,\
                       STATE        INTEGER NOT NULL);").arg(cmsCfg));
        windowConsole("创建表" + cmsCfg);
    }

    return true;
}

bool DataBase::createTableOwlh()
{
    if (!isTableExist(owlhCfg)) {
        sqlquery.exec(QString("CREATE TABLE %1(\
                       NAME      TEXT NOT NULL,\
                       WORKMODE  INTEGER NOT NULL,\
                       APPSKEY   TEXT NOT NULL,\
                       NEWSKEY   TEXT NOT NULL,\
                       CHANNEL   INTEGER NOT NULL,\
                       DEVEUI    TEXT NOT NULL,\
                       APPEUI    TEXT NOT NULL,\
                       APPKEY    TEXT NOT NULL,\
                       LOWVPT    FLOAT NOT NULL,\
                       HIGHTVPT  FLOAT NOT NULL,\
                       VAR       FLOAT NOT NULL,\
                       SAMPLETIME    INTEGER NOT NULL,\
                       HSTIME    INTEGER NOT NULL,\
                       RETRANS   INTEGER NOT NULL,\
                       TYPE         INTEGER NOT NULL,\
                       STATE     INTEGER NOT NULL);").arg(owlhCfg));
        windowConsole("创建表" + owlhCfg);
    }

    return true;
}

bool DataBase::createTableConfigRecord()
{
    if (!isTableExist(cfgRecord)) {
        sqlquery.exec(QString("CREATE TABLE %1(\
                       TIME         TEXT NOT NULL,\
                       TYPE         INTEGER NOT NULL,\
                       ADDR         TEXT NOT NULL,\
                       CONFIGFILE   TEXT NOT NULL,\
                       STATE        INTEGER NOT NULL);").arg(cfgRecord));
        windowConsole("创建表" + cfgRecord);
    }

    return true;
}

bool DataBase::saveProject(const QString &name, efsConfig &data)
{
    if (projectIsExist(name, efsCfg)) {
        projectDiscard(name, name + "(" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ")", efsCfg);
    }

    QString insert = QString("INSERT INTO %1(NAME,WORKMODE,APPSKEY,NEWSKEY,CHANNEL,DEVEUI,APPEUI,\
                                             APPKEY,HSTIME,HSNUM,RETRANS,TEMP,CURTH,PRCTH,RATIO,WRIE,STATE)\
                             VALUES(:NAME,:WORKMODE,:APPSKEY,:NEWSKEY,:CHANNEL,:DEVEUI,:APPEUI,\
                                    :APPKEY,:HSTIME,:HSNUM,:RETRANS,:TEMP,:CURTH,:PRCTH,:RATIO,:WRIE,:STATE)").arg(efsCfg);

    sqlquery.prepare(insert);
    sqlquery.bindValue(":NAME", name);
    sqlquery.bindValue(":WORKMODE", data.base.wm);
    //sqlquery->bindValue(":ADDR", data.base.addr);
    sqlquery.bindValue(":APPSKEY", data.base.appskey);
    sqlquery.bindValue(":NEWSKEY", data.base.nwskey);
    sqlquery.bindValue(":CHANNEL", data.base.channelNum);
    sqlquery.bindValue(":DEVEUI", data.deveui);
    sqlquery.bindValue(":APPEUI", data.appeui);
    sqlquery.bindValue(":APPKEY", data.appkey);
    sqlquery.bindValue(":HSTIME", data.hstime);
    sqlquery.bindValue(":HSNUM", data.hsnum);
    sqlquery.bindValue(":RETRANS", data.retrans);
    uint32_t temp = (data.tempth[0] & 0xff) << 24 | (data.tempth[1] & 0xff) << 16 |\
                    (data.tempth[2] & 0xff) << 8 | (data.tempth[3] & 0xff) << 0 ;
    sqlquery.bindValue(":TEMP", temp);
    sqlquery.bindValue(":CURTH", data.curth);
    sqlquery.bindValue(":PRCTH", data.prcth);
    sqlquery.bindValue(":RATIO", data.ratio);
    sqlquery.bindValue(":WRIE", data.wrie);
    sqlquery.bindValue(":STATE", 1);

    if (sqlquery.exec() == false) {
        windowConsole(insert);
        windowConsole(sqlquery.lastError().text());

        return false;
    }

    return true;
}

bool DataBase::selectProject(const QString &name, efsConfig &data)
{
    QString find = QString("SELECT * FROM %1 WHERE NAME IS '%2'").arg(efsCfg, name);
    if (sqlquery.exec(find) && sqlquery.next()) {
        data.base.wm = (SensorData::workMode)sqlquery.value("WORKMODE").toUInt();
        //data.base.addr = sqlquery->value("ADDR").toString();
        data.base.appskey = sqlquery.value("APPSKEY").toString();
        data.base.nwskey = sqlquery.value("NEWSKEY").toString();
        data.base.channelNum = sqlquery.value("CHANNEL").toUInt();
        data.deveui = sqlquery.value("DEVEUI").toString();
        data.appeui = sqlquery.value("APPEUI").toString();
        data.appkey = sqlquery.value("APPKEY").toString();
        data.hstime = sqlquery.value("HSTIME").toUInt();
        data.hsnum = sqlquery.value("HSNUM").toUInt();
        data.retrans = sqlquery.value("RETRANS").toUInt();
        uint32_t temp = sqlquery.value("TEMP").toUInt();
        data.tempth[0] = (temp >> 24) & 0xff;
        data.tempth[1] = (temp >> 16) & 0xff;
        data.tempth[2] = (temp >> 8) & 0xff;
        data.tempth[3] = (temp >> 0) & 0xff;
        data.curth = sqlquery.value("CURTH").toUInt();
        data.prcth = sqlquery.value("PRCTH").toUInt();
        data.ratio = sqlquery.value("RATIO").toUInt();
        data.wrie = sqlquery.value("WRIE").toUInt();

        return true;
    }

    return false;
}

bool DataBase::saveProject(const QString &name, cmsConfig &data)
{
    if (projectIsExist(data.st, name, cmsCfg)) {
        projectDiscard(data.st, name, name + "(" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ")", cmsCfg);
    }

    QString insert = QString("INSERT INTO %1(NAME,WORKMODE,APPSKEY,NEWSKEY,CHANNEL,LOWVPT,HIGHTVPT,\
                                             VAR,LOWHUMIDITY,HIGHTHUMIDITY,SAMPLE,UPINTERVAL,RETRANS,TYPE,STATE)\
                             VALUES(:NAME,:WORKMODE,:APPSKEY,:NEWSKEY,:CHANNEL,:LOWVPT,:HIGHTVPT,\
                                    :VAR,:LOWHUMIDITY,:HIGHTHUMIDITY,:SAMPLE,:UPINTERVAL,:RETRANS,:TYPE,:STATE)").arg(cmsCfg);

    sqlquery.prepare(insert);
    sqlquery.bindValue(":NAME", name);
    sqlquery.bindValue(":WORKMODE", data.base.wm);
    //sqlquery->bindValue(":ADDR", data.base.addr);
    sqlquery.bindValue(":APPSKEY", data.base.appskey);
    sqlquery.bindValue(":NEWSKEY", data.base.nwskey);
    sqlquery.bindValue(":CHANNEL", data.base.channelNum);

    sqlquery.bindValue(":LOWVPT", data.lowtv);
    sqlquery.bindValue(":HIGHTVPT", data.hightv);
    sqlquery.bindValue(":VAR", data.variation);
    sqlquery.bindValue(":LOWHUMIDITY", data.lowhumidity);
    sqlquery.bindValue(":HIGHTHUMIDITY", data.highhumidity);
    sqlquery.bindValue(":SAMPLE", data.sampleTime);
    sqlquery.bindValue(":UPINTERVAL", data.upTransTime);
    sqlquery.bindValue(":RETRANS", data.reTrans);
    sqlquery.bindValue(":TYPE", data.st);
    sqlquery.bindValue(":STATE", 1);

    if (sqlquery.exec() == false) {
        windowConsole(insert);
        windowConsole(sqlquery.lastError().text());

        return false;
    }

    return true;
}

bool DataBase::selectProject(SensorData::sensorType type, const QString &name, cmsConfig &data)
{
    QString find = QString("SELECT * FROM %1 WHERE NAME IS '%2' AND TYPE IS '%3'").arg(cmsCfg, name).arg(type);
    if (sqlquery.exec(find) && sqlquery.next()) {
        data.base.wm = (SensorData::workMode)sqlquery.value("WORKMODE").toUInt();
        data.base.appskey = sqlquery.value("APPSKEY").toString();
        data.base.nwskey = sqlquery.value("NEWSKEY").toString();
        data.base.channelNum = sqlquery.value("CHANNEL").toUInt();
        data.lowtv = sqlquery.value("LOWVPT").toFloat();
        data.hightv = sqlquery.value("HIGHTVPT").toFloat();
        data.variation = sqlquery.value("VAR").toFloat();
        data.lowhumidity = sqlquery.value("LOWHUMIDITY").toFloat();
        data.highhumidity = sqlquery.value("HIGHTHUMIDITY").toFloat();
        data.lowtv = sqlquery.value("LOWVPT").toFloat();
        data.sampleTime = sqlquery.value("SAMPLE").toUInt();
        data.upTransTime = sqlquery.value("UPINTERVAL").toUInt();
        data.reTrans = sqlquery.value("RETRANS").toUInt();
        data.st = (SensorData::sensorType)sqlquery.value("TYPE").toUInt();

        return true;
    }

    return false;
}

bool DataBase::saveProject(const QString &name, OWLHConfig &data)
{
    if (projectIsExist(data.st, name, owlhCfg)) {
        projectDiscard(data.st, name, name + "(" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ")", owlhCfg);
    }

    QString insert = QString("INSERT INTO %1(NAME,WORKMODE,APPSKEY,NEWSKEY,CHANNEL,DEVEUI,APPEUI,\
                             APPKEY,LOWVPT,HIGHTVPT,VAR,SAMPLETIME,HSTIME,RETRANS,RETRANS,TYPE,STATE)\
                             VALUES(:NAME,:WORKMODE,:APPSKEY,:NEWSKEY,:CHANNEL,:DEVEUI,:APPEUI,\
                            :APPKEY,:LOWVPT,:HIGHTVPT,:VAR,:SAMPLETIME,:HSTIME,:RETRANS,:RETRANS,:TYPE,:STATE)").arg(owlhCfg);

    sqlquery.prepare(insert);
    sqlquery.bindValue(":NAME", name);
    sqlquery.bindValue(":WORKMODE", data.base.wm);
    sqlquery.bindValue(":APPSKEY", data.base.appskey);
    sqlquery.bindValue(":NEWSKEY", data.base.nwskey);
    sqlquery.bindValue(":CHANNEL", data.base.channelNum);
    sqlquery.bindValue(":DEVEUI", data.deveui);
    sqlquery.bindValue(":APPEUI", data.appeui);
    sqlquery.bindValue(":APPKEY", data.appkey);
    sqlquery.bindValue(":LOWVPT", data.lowth);
    sqlquery.bindValue(":HIGHTVPT", data.highth);
    sqlquery.bindValue(":VAR", data.variation);
    sqlquery.bindValue(":SAMPLETIME", data.sampletime);
    sqlquery.bindValue(":HSTIME", data.upintervalTime);
    sqlquery.bindValue(":RETRANS", data.retrans);
    sqlquery.bindValue(":TYPE", data.st);
    sqlquery.bindValue(":STATE", 1);

    if (sqlquery.exec() == false) {
        windowConsole(insert);
        windowConsole(sqlquery.lastError().text());

        return false;
    }

    return true;
}

bool DataBase::selectProject(SensorData::sensorType type, const QString &name, OWLHConfig &data)
{
    QString find = QString("SELECT * FROM %1 WHERE NAME IS '%2' AND TYPE IS '%3'").arg(owlhCfg, name).arg(type);
    if (sqlquery.exec(find) && sqlquery.next()) {
        data.base.wm = (SensorData::workMode)sqlquery.value("WORKMODE").toUInt();
        data.base.appskey = sqlquery.value("APPSKEY").toString();
        data.base.nwskey = sqlquery.value("NEWSKEY").toString();
        data.base.channelNum = sqlquery.value("CHANNEL").toUInt();
        data.deveui = sqlquery.value("DEVEUI").toString();
        data.appeui = sqlquery.value("APPEUI").toString();
        data.appkey = sqlquery.value("APPKEY").toString();
        data.lowth = sqlquery.value("LOWVPT").toFloat();
        data.highth = sqlquery.value("HIGHTVPT").toFloat();
        data.variation = sqlquery.value("VAR").toFloat();
        data.sampletime = sqlquery.value("SAMPLETIME").toUInt();
        data.upintervalTime = sqlquery.value("HSTIME").toUInt();
        data.retrans = sqlquery.value("RETRANS").toUInt();
        data.st = (SensorData::sensorType)sqlquery.value("TYPE").toUInt();

        return true;
    }

    return false;
}

bool DataBase::projectDiscard(const QString &oldname, const QString &newname, const QString &table)
{
    QString update = QString("UPDATE %1 SET NAME = '%2' , STATE = '0' WHERE NAME = '%3';"\
                         ).arg(table, newname, oldname);

    return sqlquery.exec(update);
}

bool DataBase::projectDiscard(SensorData::sensorType type, const QString &oldname, const QString &newname, const QString &table)
{
    QString update = QString("UPDATE %1 SET NAME = '%2' , STATE = '0' WHERE NAME = '%3' AND TYPE IS '%4';"\
                         ).arg(table, newname, oldname).arg(type);

    return sqlquery.exec(update);
}

bool DataBase::projectIsExist(const QString &name, const QString &table)
{
    QString find = QString("SELECT * FROM %1 WHERE NAME IS '%2'").arg(table, name);
    if (sqlquery.exec(find) && sqlquery.next()) {
        return true;
    }
    return false;
}

bool DataBase::projectIsExist(SensorData::sensorType type, const QString &name, const QString &table)
{
    QString find = QString("SELECT * FROM %1 WHERE NAME IS '%2' AND TYPE IS '%3'").arg(table, name).arg(type);
    if (sqlquery.exec(find) && sqlquery.next()) {
        return true;
    }
    return false;
}

bool DataBase::selectProjectList(QStringList &pl)
{
    QString find = QString("SELECT NAME FROM %1 WHERE STATE IS '1'").arg(efsCfg);
    sqlquery.exec(find);

    while (sqlquery.next()) {
        pl << sqlquery.value("NAME").toString();
    }

    return true;
}

bool DataBase::selectProjectList(SensorData::sensorType type, QStringList &pl)
{
    QString find;

    if (type == SensorData::EFS ||\
        type == SensorData::NPWS) {
        selectProjectList(pl);
        return true;
    }
    if (type == SensorData::OWDS ||\
        type == SensorData::OWPS) {
        find = QString("SELECT NAME FROM %1 WHERE STATE IS '1' AND TYPE IS '%2'").arg(owlhCfg).arg(type);
    }
    else {
        find = QString("SELECT NAME FROM %1 WHERE STATE IS '1' AND TYPE IS '%2'").arg(cmsCfg).arg(type);
    }
    sqlquery.exec(find);

    while (sqlquery.next()) {
        pl << sqlquery.value("NAME").toString();
    }

    return true;
}

bool DataBase::save(SensorData::configRecord &data)
{
    QString find = QString("SELECT * FROM %1 WHERE ADDR IS '%2'").arg(cfgRecord, data.addr);
    if (sqlquery.exec(find) && sqlquery.next()) {
        QString update = QString("UPDATE %1 SET TIME = '%2', TYPE = '%3', CONFIGFILE  = '%4',\
                                    STATE  = '%5' WHERE ADDR = '%6';"\
                             ).arg(cfgRecord, data.time).arg(data.type).arg(data.cfile).arg(data.state).arg(data.addr);

        return sqlquery.exec(update);
    }

    QString insert = QString("INSERT INTO %1(TIME,TYPE,ADDR,CONFIGFILE,STATE)\
                             VALUES(:TIME,:TYPE,:ADDR,:CONFIGFILE,:STATE)").arg(cfgRecord);

    sqlquery.prepare(insert);
    sqlquery.bindValue(":TIME", data.time);
    sqlquery.bindValue(":TYPE", data.type);
    sqlquery.bindValue(":ADDR", data.addr);
    sqlquery.bindValue(":CONFIGFILE", data.cfile);
    sqlquery.bindValue(":STATE", data.state);
    if (sqlquery.exec() == false) {
        windowConsole(insert);
        windowConsole(sqlquery.lastError().text());

        return false;
    }

    return true;
}

bool DataBase::foreachConfigRecord()
{
    QString find = QString("SELECT * FROM %1").arg(cfgRecord);
    sqlquery.exec(find);

    while (sqlquery.next()) {
        SensorData::configRecord data;
        data.time = sqlquery.value("TIME").toString();
        data.type = (SensorData::sensorType)sqlquery.value("TYPE").toUInt();
        data.addr = sqlquery.value("ADDR").toString();
        data.cfile = sqlquery.value("CONFIGFILE").toString();
        data.state = sqlquery.value("STATE").toUInt();
        emit selectConfigRecord(data);
    }

    return true;
}

bool DataBase::copyTable(const QString &name, const QString &nameCopy)
{
    QString cpoy = QString("CREATE TABLE %1 AS SELECT * FROM %2").arg(nameCopy, name);

    return sqlquery.exec(cpoy);
}

bool DataBase::clearTable(const QString &name)
{
    QString del = QString("DELETE FROM %1").arg(name);

    return sqlquery.exec(del);
}

bool DataBase::resetConfigRecord()
{
    if (isEmpty(cfgRecord)) {
        windowConsole(cfgRecord + "表内无数据");
        return false;
    }

    if (copyTable(cfgRecord, cfgRecord + QDateTime::currentDateTime().toString("_yyyy_MM_dd_hh_mm_ss"))) {
        return clearTable(cfgRecord);
    }

    return false;
}

bool DataBase::isEmpty(const QString &tableName)
{
    QString find = QString("SELECT * FROM %1").arg(tableName);
    sqlquery.exec(find);

    while (sqlquery.next()) {
        return false;
    }

    return true;
}
