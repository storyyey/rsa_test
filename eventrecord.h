#ifndef EVENTRECORD_H
#define EVENTRECORD_H

#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QLayout>
#include <QPushButton>
#include <QListWidget>
#include <QProgressBar>
#include <QByteArray>
#include <QScrollBar>
#include <QTimer>
#include <QLabel>

class EventRecord : public QWidget
{
    Q_OBJECT
public:
    static QString sensorSelectObjName;
    static QString presetSelectObjName;
    static bool runningFlag;
    static QMap<QString, QWidget *> eventObjects;

    explicit EventRecord(QWidget *parent = nullptr);
    static void addEventObj(QLineEdit *obj, QObject *parent, const QString &objname);
    static QString decodeEventObjValue(QLineEdit *obj, const QString &value);
    static void addEventObj(QPushButton *obj, QObject *parent, const QString &objname);
    static QString decodeEventObjValue(QPushButton *obj);
    static void addEventObj(QCheckBox *obj, QObject *parent, const QString &objname);
    static QString decodeEventObjValue(QCheckBox *obj, int state);
    static void addEventObj(QComboBox *obj, QObject *parent, const QString &objname);
    static QString decodeEventObjValue(QComboBox *obj, const QString &value);
    static void addEventObjValue(QComboBox *obj, const QString &value);
    static bool readEventLog();
    void showWidget();
    bool addRecordItem(const QString &data);
    void play();
    void stopPlay();
    void next();
    void prev();
    void closeEvent(QCloseEvent *event);
    void setIni();
    void setFoucusStyle(bool flag);
    bool parse(const QString &data);
    void hangUp();
    void continued();
    static QString objName(const QString &prefix, const QString &name);
private:
    QListWidget *recordList;
    QSlider *slider;
    QTimer *playTimer;
    QPushButton *playBtn;
    QMap<QString, int> speedMap;
    QComboBox *playSpeed;
    bool sliderIsPress = false;
    QLineEdit *displayCount;
    QStringList hangUpData;
signals:

};

#endif // EVENTRECORD_H
