#ifndef SENSORCONFIGWIDGET_H
#define SENSORCONFIGWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QLayout>
#include <QMap>
#include <QMessageBox>
#include <QStackedWidget>
#include <QPlainTextEdit>
#include "sensordata.h"
#include "sensorbaseconfig.h"
#include "electricalfiresensor.h"
#include "database.h"
#include "saveasdialog.h"
#include "promptdialog.h"
#include "tableswidget.h"
#include "commonsensor.h"
#include "outdoorwlhs.h"
#include "temperaturesensor.h"
#include "humituresensor.h"
#include "waterpresssensor.h"
#include "waterlevelsensor.h"
#include "switchvalsensor.h"
#include "threephasevoltagesensor.h"
#include "threephaseelectricity.h"
#include "residualelectricitysensor.h"
#include "threephaseswitchsensor.h"
#include "waterpumpsensor.h"
#include "eventrecord.h"

class SensorConfigWidget : public QWidget
{
    Q_OBJECT
public:
    enum ConfigMode {
        MANNUL = 0,
        AUTO = 1,
    };
    explicit SensorConfigWidget(QWidget *parent = nullptr);
    ~SensorConfigWidget();
    SensorConfigWidget(TablesWidget *tables, DataBase *DB);
    void widgetInit();
    void sensorTypeComboxRefsh();
    void setATcmd(ATCmd *parentAtCmd);
    void presetProjectResh();
    void setPromptDialog(PromptDialog *prompt);
    void setConfigMode(SensorConfigWidget::ConfigMode mode);
    SensorConfigWidget::ConfigMode getConfigMode();
    void startConfig(const QString &addr);
    void stopConfig();
    void sensorWidgetMapInit();
    SensorData::sensorType currType();
    void setini();
    void eventRecordInit();
private:
    QString iniMainGroup = "SensorConfigWidget";
    QString configfile;
    TablesWidget *tables = nullptr;
    PromptDialog *prompt = nullptr;
    DataBase *parentDB = nullptr;
    SaveAsDialog *savePreset = nullptr;
    ElectricalFireSensor *electricalFireSensor = nullptr;
    TemperatureSensor *temperatureSensor = nullptr;
    HumitureSensor *humituresensor = nullptr;
    WaterPressSensor *waterpresssensor = nullptr;
    WaterLevelSensor *waterlevelsensor = nullptr;
    SwitchValSensor *switchvalsensor = nullptr;
    ThreephaseVoltageSensor *threephasevoltagesensor = nullptr;
    ThreephaseElectricity *threephaseelectricity = nullptr;
    ResidualElectricitySensor *residualelectricitysensor = nullptr;
    ThreephaseSwitchSensor *threephaseswitchsensor = nullptr;
    WaterpumpSensor *waterpumpsensor = nullptr;
    OutdoorWLHS *owlhSensor = nullptr;
    QString presetBoxDefVal = "-- 未选择 --";
    QComboBox *presetValBox;
    QVBoxLayout *mainLayout;
    QComboBox *sensorTypeCombox;
    QList<SensorData::sensorType> sensorList = {SensorData::TES, SensorData::THS,\
                                                SensorData::WPS, SensorData::WDS,\
                                                SensorData::SNO, SensorData::PVM,\
                                                SensorData::PCM, SensorData::PRC,\
                                                SensorData::PSD, SensorData::EFS,\
                                                SensorData::OWPS, SensorData::OWDS,\
                                                SensorData::NPWS};
    QMap<SensorData::sensorType, QWidget *> configEditWidget;
    QMap<SensorData::sensorType, SensorOperation *> SensorOperator;
    ConfigMode mode = MANNUL;
    QPushButton *operMode;
    ATCmd *parentAtCmd = nullptr;
    QStackedWidget *typeStacke;
signals:

private slots:
      void selectSensorChange(const QString &text);
      void saveBtnSlot();
      void saveAsProject();
      void changeModeBtnSlot();
      void presetProjectChange(int index);
      void cannelConfig();
      void newDevicesInfo(const QString &addr);
      void atCmdFinish(ATCmd::finishState state);
      void saveAsConfirm(const QString &name);
public slots:
      void activeSlot(int type);
};

#endif // SENSORCONFIGWIDGET_H
