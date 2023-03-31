#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QString>
#include <QtSql>
#include <QStringList>
#include "sensordata.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();
    bool isTableExist(const QString &name);
    bool createTableEfs();
    bool createTableCms();
    bool createTableOwlh();
    bool createTableConfigRecord();
    bool saveProject(const QString &name, efsConfig &data);
    bool selectProject(const QString &name, efsConfig &data);
    bool saveProject(const QString &name, cmsConfig &data);
    bool selectProject(SensorData::sensorType type, const QString &name, cmsConfig &data);
    bool saveProject(const QString &name, OWLHConfig &data);
    bool selectProject(SensorData::sensorType type, const QString &name, OWLHConfig &data);
    bool projectDiscard(const QString &oldname, const QString &newname, const QString &table);
    bool projectDiscard(SensorData::sensorType type, const QString &oldname, const QString &newname, const QString &table);
    bool projectIsExist(const QString &name, const QString &table);
    bool projectIsExist(SensorData::sensorType type, const QString &name, const QString &table);
    bool selectProjectList(QStringList &pl);
    bool selectProjectList(SensorData::sensorType type, QStringList &pl);
    bool save(SensorData::configRecord &data);
    bool foreachConfigRecord();
    bool copyTable(const QString &name, const QString &nameCopy);
    bool clearTable(const QString &name);
    bool resetConfigRecord();
    bool isEmpty(const QString &tableName);
private:
    QSqlDatabase database;
    QSqlQuery sqlquery;
    QString findTableSql = "select name from sqlite_master where name=\'%1\';";
    QString efsCfg = "EFSCFG";
    QString cmsCfg = "CMSCFG";
    QString owlhCfg = "OWLHCFG";
    QString cfgRecord = "CONFIGRECORD";
signals:
    void selectConfigRecord(SensorData::configRecord &data);
};

#endif // DATABASE_H
