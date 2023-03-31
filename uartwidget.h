#ifndef UARTWIDGET_H
#define UARTWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include "atcmd.h"
#include "promptdialog.h"

class UartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UartWidget(QWidget *parent = nullptr);
    ~UartWidget();
    void portListRefresh();
    bool openPort(const QString &name, QSerialPort *serial);
    bool closePort(QSerialPort *serial);
    void setATcmd(ATCmd *ac);
    void setCurrPortListText(const QString &name, QComboBox *box);
    void setConfigProgressDialog(PromptDialog *cpd);
    void initNewDevTips();
    void sensorStandHint(bool flag);
    void barcodeStandHint(bool flag);
    void helpDialogInit();
private:
    QString openStatIcon = ":/icon/open.png";
    QString closeStatIcon = ":/icon/close.png";
    QString standStatIcon = ":/icon/stand.png";
    QString sendRunStatIcon = ":/icon/sendrun.png";

    PromptDialog *configProgressDialog;
    QComboBox *sensorPortComBox;
    QComboBox *barcodePortComBox;
    QSerialPort *sensorSerial;
    QSerialPort *barcodeSerial;
    QPushButton *sensorPortState;
    QPushButton *sensorUartBtn;
    QPushButton *barcodePortState;
    QPushButton *barcodeUartBtn;
    ATCmd *ac;
    QTimer *refhPortTimer;

    QDialog *newDevTips;
    QStringList portList;
    QString newDevName;
    QLabel *tipsLabel;
    QString btnStyle;

    bool sensorStandState = false;
    bool barcodeStandState = false;
    QTimer *standStateTimer;
    QTimer *sensorKeepTimer;
    QTimer *barcodeKeepTimer;

    QDialog *helpDialog;
signals:
    void barcodeData(const QString &data);
private slots:
    void sensorUartBtnClick();
    void barcodeUartBtnClick();
    void readSeneorInfo();
    void readBarcodeInfo();
    void sendToSensor(const QString &cmd);
    void CmdFinish(ATCmd::finishState state);
    void autoRefreshPortTimer();
};

#endif // UARTWIDGET_H
