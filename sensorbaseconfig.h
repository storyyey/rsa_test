#ifndef SENSORBASECONFIG_H
#define SENSORBASECONFIG_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QLayout>
#include <QMap>
#include "sensordata.h"
#include "sensoroperation.h"
#include "eventrecord.h"

class sensorBaseConfig : public QWidget, public SensorOperation
{
    Q_OBJECT
public:
    explicit sensorBaseConfig(QWidget *parent = nullptr);
    ~sensorBaseConfig();
    QCheckBox *abpMode;
    QCheckBox *otaaMode;
    QComboBox *addrCombox;
    QLineEdit *addrLineEdit;
    QStringList addrHistory;
    QLineEdit *askLineEdit;
    QLineEdit *nskLineEdit;
    QComboBox *channelSelect;
    uint32_t channelMax = 11;
    bool getConfig(SensorData::sensorBaseConfig &data);
    bool setConfig(SensorData::sensorBaseConfig &data);
    void setEnabled(bool b);
    bool setDevAddr(const QString &addr);
    void addAddrHistory(const QString &addr);
    bool installEventRecord(const QString &prefix);
signals:
private slots:
      void ABPModeCheck(int state);
      void OTAAModeCheck(int state);
};

#endif // SENSORBASECONFIG_H
