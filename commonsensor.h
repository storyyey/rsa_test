#ifndef COMMONSENSOR_H
#define COMMONSENSOR_H

#include <QWidget>
#include "sensordata.h"
#include "sensorbaseconfig.h"
#include "database.h"
#include "saveasdialog.h"
#include "promptdialog.h"
#include "tableswidget.h"
#include "eventrecord.h"

class CommonSensor : public QWidget, public SensorOperation
{
    Q_OBJECT
public:
    enum devaddrLen { DEVADDR_LENGTH = 8};
    explicit CommonSensor(QWidget *parent = nullptr);
    ~CommonSensor();
    void baseWdigetInit();
    void thresholdWidgetInit();
    void sleepWidgetInit();
    void calibrationWidgetInit();
    bool getConfig(cmsConfig &cfg);
    bool setConfig(cmsConfig &cfg);
    void setEnabled(bool b);
    QString parserDevAddr(const QString &text);
    QString getDevAddr();
    bool setDevAddr(const QString &addr);
    bool isValidConfig(QString *err = nullptr);
    void widgetsShow(QList<QWidget *> &ws);
    void widgetsHide(QList<QWidget *> &ws);
    bool IsModify(SensorData::sensorType type, const QString &text);
    bool saveAs(SensorData::sensorType type, const QString &name);
    bool load(SensorData::sensorType type, const QString &name);
    void LowThresholdEdit(bool b);
    void HighThresholdEdit(bool b);
    void humidityEdit(bool b);
    void variationEnableEdit(bool b);
    bool cancelApply();
    bool apply();
    bool installEventRecord(const QString &prefix);
private:
    QWidget *mainWidget;
    sensorBaseConfig *sbaseWidget;
    QVBoxLayout *mainLayout;

    QLineEdit *lowthEdit;
    QLabel *lowthUnitLabel;
    QList<QWidget *> lowthWidget;

    QLineEdit *lowhumidity;
    QLabel *lowhumidityLabel;
    QList<QWidget *> lowhumidityWidget;

    QLineEdit *highthEdit;
    QLabel *highthUnitLabel;
    QList<QWidget *> highthWidget;

    QLineEdit *highthumidity;
    QLabel *highthumidityLabel;
    QList<QWidget *> highthumidityWidget;

    QLineEdit *variationEdit;
    QLabel *variationUnitLabel;
    QList<QWidget *> varWidget;

    QLineEdit *sampleEdit;
    QLineEdit *upIntervalEdit;
    QLineEdit *retransEdit;
signals:

};

#endif // COMMONSENSOR_H
