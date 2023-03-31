#ifndef OUTDOORWLHS_H
#define OUTDOORWLHS_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QMap>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "sensordata.h"
#include "sensorbaseconfig.h"
#include "atcmd.h"
#include "sensoroperation.h"
#include "eventrecord.h"

class OutdoorWLHS : public QWidget, public SensorOperation
{
    Q_OBJECT
public:
    explicit OutdoorWLHS(QWidget *parent = nullptr);
    ~OutdoorWLHS();
    void setEnabled(bool b);
    bool getConfig(OWLHConfig &cfg);
    bool setConfig(OWLHConfig &cfg);
    bool cancelApply();
    bool apply();
    QString parserDevAddr(const QString &text);
    QString getDevAddr();
    bool setDevAddr(const QString &addr);
    bool isValidConfig(QString *err = nullptr);
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &text);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
    void eventRecordInit();
    bool installEventRecord(const QString &prefix);
private:
    SensorData::sensorType sensorType = SensorData::NON;
    sensorBaseConfig *sbase;
    QVBoxLayout *mainLayout;
    QLineEdit *deveuiEdit;
    QLineEdit *appeuiEdit;
    QLineEdit *appkeyEdit;
    QLineEdit *lowthEdit;
    QLineEdit *highthEdit;
    QLineEdit *varthEdit;
    QLineEdit *sampleTimeEdit;
    QLineEdit *upintervalEdit;
    QComboBox *upintervalBox;
    QLineEdit *reTransEdit;
    QComboBox *displayModeBox;
    QList<uint32_t> upintervalPreset = {1800, 3600, 7200, 14400, 28800, 43200, 84600};
signals:

};

#endif // OUTDOORWLHS_H
