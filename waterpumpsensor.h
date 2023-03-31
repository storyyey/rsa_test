#ifndef WATERPUMPSENSOR_H
#define WATERPUMPSENSOR_H

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
#include "steptips.h"
#include "eventrecord.h"

class WaterpumpSensor : public QWidget, public SensorOperation
{
    Q_OBJECT
public:
    enum devaddrLen { DEVADDR_LENGTH = 8};
    explicit WaterpumpSensor(QWidget *parent = nullptr);
    ~WaterpumpSensor();
    void setEnabled(bool b);
    bool getConfig(efsConfig &cfg);
    bool setConfig(efsConfig &cfg);
    bool cancelApply();
    bool apply();
    QString parserDevAddr(const QString &text);
    QString getDevAddr();
    bool setDevAddr(const QString &addr);
    bool isValidConfig(QString *err = nullptr);
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &text);
    void connectAtCmdFinish();
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
    bool installEventRecord(const QString &prefix);
private:
#if 0
    StepTips *steptips;
#endif
    QMessageBox *operMsg;
    sensorBaseConfig *sbase;
    QVBoxLayout *mainLayout;
    QLineEdit *deveuiEdit;
    QLineEdit *appeuiEdit;
    QLineEdit *appkeyEdit;
    QComboBox *hsTimeEdit;
    QList<uint32_t> hsList = {1800, 3600, 7200, 14400, 28800, 43200, 84600};
    QLineEdit *hsNumEdit;
    QLineEdit *reTransEdit;
    QLineEdit *channelTh1Edit;
    QLineEdit *channelTh2Edit;
    QLineEdit *channelTh3Edit;
    QLineEdit *channelTh4Edit;
    QLineEdit *curthEdit;
    QLineEdit *prcthEdit;
    QComboBox *ratioEdit;
    QList<QString> ratioList = {"75A/0.333V", "150A/0.333V", "250A/0.333V", "500A/0.333V"};
    QComboBox *wireEdit;
    QList<QString> wireList = {"单相二线制", "三相四线制"};

#if 0
    QWidget *manualOperWidget;
    QPushButton *enterATmode;
    QPushButton *vpgainBtn;
    QPushButton *cpgainBtn;
    QPushButton *cngainBtn;
    QPushButton *cpoffsetBtn;
    QPushButton *cnoffsetBtn;
    bool inManualOperation = false;
    QString hintMessage;
#endif
signals:

private slots:
    //void vpgainBtnSlot();
    //void cpgainBtnSlot();
    //void cngainBtnSlot();
    //void cpoffsetBtnSlot();
    //void cnoffsetBtnSlot();
    //void enterAtModeBtnSlot();
    //void atCmdFinish(ATCmd::finishState state);
};

#endif // WATERPUMPSENSOR_H
