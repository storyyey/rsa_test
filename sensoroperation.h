#ifndef SENSOROPERATION_H
#define SENSOROPERATION_H

#include "sensordata.h"
#include "atcmd.h"
#include "database.h"
#include <QObject>

class SensorOperation
{
public:
    template<typename T>
    bool getConfig(T &data);
    template<typename T>
    bool setConfig(T &data);
    virtual bool apply(){return false;}
    virtual bool cancelApply(){return false;}
    virtual QString getDevAddr() {return "";}
    virtual bool setDevAddr(const QString &addr) {QString devaddr = addr; return false;}
    virtual bool isValidConfig(QString *err = nullptr) {if (err) *err = ""; return false;}
    virtual QString parserDevAddr(const QString &text) {QString str = text; return "";}
    virtual void setATcmd(ATCmd *ac) { this->ac = ac;}
    virtual ATCmd *getATcmd() { return this->ac;}
    virtual void setEnabled(bool b) {if (b) return ;}
    virtual void setDefConfig(SensorData::sensorType sensorType = SensorData::NON) {if (sensorType == SensorData::NON) return;}
    virtual void setDB(DataBase *db) {this->db = db;}
    virtual DataBase *getDB() {return this->db;}
    virtual bool presetIsModify(const QString &text) {QString str = text; return false;}
    virtual bool saveAsPreset(const QString &name) {QString str = name; return false;}
    virtual bool loadPreset(const QString &name) {QString str = name; return false;}
private:
    DataBase *db = NULL;
    ATCmd *ac = NULL;
};

#endif // SENSOROPERATION_H
