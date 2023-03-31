#include "eventrecord.h"
#include <QDebug>
#include <QEvent>
#include <QMap>
#include <settingOption.h>
#include <QScreen>

bool EventRecord::runningFlag = false;
QString EventRecord::sensorSelectObjName = "传感器类型选择";
QString EventRecord::presetSelectObjName = "预设配置选择";
QMap<QString, QWidget *> EventRecord::eventObjects;

inline bool appendEventWidgets(QWidget *w)
{
    if (!w || !w->objectName().size())
        return false;

    if (EventRecord::eventObjects.contains(w->objectName())) {
        qDebug() << w->objectName() + " already existed ";
        return false;
    }

    qDebug() << w->objectName() + " append success" ;

    EventRecord::eventObjects[w->objectName()] = w;

    return true;
}

inline bool writeLog(const QString &msg)
{
    if (EventRecord::runningFlag) return false;

    QFile logFile(QDir::currentPath() + "/userOperateLog.json");
    if (logFile.size() > 1024 * 1024) {
        qDebug() << "文件备份";
        logFile.rename(QDir::currentPath() + "/userOperateLog" + \
                       QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".json");
        logFile.setFileName(QDir::currentPath() + "/userOperateLog.json");
    }
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "文件打开失败";
        return false;
    }

    logFile.write(msg.toUtf8());
    logFile.write("\n");
    logFile.close();
    qDebug() << msg;

    return true;
}

inline bool writeLog(QJsonDocument &msg)
{
    return writeLog(msg.toJson(QJsonDocument::Compact));
}

inline bool readLogLines(QStringList &logLine, int cnt = 1000)
{
    QFile logFile(QDir::currentPath() + "/userOperateLog.json");
    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "文件打开失败";
        return false;
    }

    QByteArray lineData;
    do {
        lineData = logFile.readLine();
        if (!(lineData.size() > 0)) {
            break;
        }

        qDebug() << lineData;
        logLine << lineData;
        if (logLine.size() > cnt) {
            logLine.takeFirst();
        }
    } while (lineData.size() > 0);

    logFile.close();

    return true;
}

EventRecord::EventRecord(QWidget *parent) : QWidget(parent)
{
    QString style = "QComboBox {\
            border-radius:4px;\
            height: 30px;\
         }";
    style += "QLineEdit {\
        border-radius:4px;\
        height: 30px;\
      }";
    style += ".QWidget {\
        background-color: #F8F8FF;\
        border:2px solid #63B8FF;\
        border-radius: 10px;\
     }";
    style += "QPushButton:hover {\
        background-color: #EBEBEB;\
     }"\
    "QPushButton:pressed {\
        background-color: #F8F8FF;\
    }";
    style += "QPushButton {\
            background-color: #FAFAFA;\
            border-radius: 10px;\
            height: 35px;\
          }";
    style +=
           "QListWidget {\
            border: 0px;\
            outline:0px;\
            background-color: #FFFFFF;\
           }"\
          "QListWidget::item {\
            border-radius: 4px;\
            height: 40px;\
            border: 1px solid #F0F0F0;\
          }"\
          "QListWidget::item:selected {\
            background:#FFF68F;\
            color:#000000;\
          }";
    this->setStyleSheet(style);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowTitle("回溯跟踪");
    QVBoxLayout *thisLayout = new QVBoxLayout();
    this->setLayout(thisLayout);

    playTimer = new QTimer();
    connect(playTimer, &QTimer::timeout, this, [this] {
        play();
        next();
        /* 自动停止 */
        if (slider->value() == slider->minimum()) {
            stopPlay();
        }
    });

    QHBoxLayout *operLayout = new QHBoxLayout();
    thisLayout->addLayout(operLayout);

    QPushButton *prevBtn = new QPushButton();
    prevBtn->setIcon(QIcon(":/icon/prev.png"));
    operLayout->addWidget(prevBtn);
    connect(prevBtn, &QPushButton::clicked, this, [this] {
        next();
        play();
    });

    playBtn = new QPushButton();
    playBtn->setIcon(QIcon(":/icon/start.png"));
    operLayout->addWidget(playBtn);
    connect(playBtn, &QPushButton::clicked, this, [this] {
        if (playTimer->isActive()) {
            stopPlay();
        }
        else {
            int inter = speedMap[playSpeed->currentText()];
            inter = inter > 200 ? inter : 200;
            playTimer->setInterval(inter);
            playTimer->start();
            playBtn->setIcon(QIcon(":/icon/pause.png"));
        }
    });

    QPushButton *nextBtn = new QPushButton();
    nextBtn->setIcon(QIcon(":/icon/next.png"));
    operLayout->addWidget(nextBtn);
    connect(nextBtn, &QPushButton::clicked, this, [this] {
        prev();
        play();
    });

    playSpeed = new QComboBox();
    playSpeed->setFixedWidth(100);
    playSpeed->addItems(QStringList() << "快速x2"<< "快速" <<"正常" << "慢速" << "慢速x2" );
    speedMap.insert("快速x2", 100);
    speedMap.insert("快速", 200);
    speedMap.insert("正常", 500);
    speedMap.insert("慢速", 1000);
    speedMap.insert("慢速x2", 2000);
    playSpeed->setCurrentText("正常");
    operLayout->addWidget(playSpeed);
    connect(playSpeed, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        int inter = speedMap[text];
        inter = inter > 100 ? inter : 100;
        playTimer->setInterval(inter);
    });

    QRegExp dcExp("((1|2)\\d{4}|\\d{0,4})");
    QValidator *dcvalidator = new QRegExpValidator(dcExp, this);
    displayCount = new QLineEdit();
    displayCount->setValidator(dcvalidator);
    displayCount->setFixedWidth(140);
    displayCount->setText("1000");
    operLayout->addWidget(displayCount);

    QHBoxLayout *sliderLayout = new QHBoxLayout();
    thisLayout->addLayout(sliderLayout);

    QString stateBtn = "QPushButton{background-color: transparent;}";
    QPushButton *newLabel = new QPushButton();
    newLabel->setIcon(QIcon(":/icon/new.png"));
    newLabel->setStyleSheet(stateBtn);
    sliderLayout->addWidget(newLabel);
    slider = new QSlider(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksAbove);
    sliderLayout->addWidget(slider);
    QPushButton *oldLabel = new QPushButton();
    oldLabel->setIcon(QIcon(":/icon/history.png"));
    oldLabel->setStyleSheet(stateBtn);
    sliderLayout->addWidget(oldLabel);

    connect(slider, &QSlider::sliderPressed, this, [this] {
        this->sliderIsPress = true;
    });
    connect(slider, &QSlider::sliderReleased, this, [this] {
        this->sliderIsPress = false;
    });

    recordList = new QListWidget();
    QScrollBar *scrollbar = recordList->verticalScrollBar();
    connect(slider, &QSlider::valueChanged, this, [this, scrollbar, oldLabel](int val){
        recordList->setCurrentRow(val);
        oldLabel->setText(QString("%1/%2").arg(slider->value() + 1).arg(slider->maximum() + 1));
        if (sliderIsPress) {
            scrollbar->setValue(val);
        }
    });

    thisLayout->addWidget(recordList);
    connect(recordList, &QListWidget::currentRowChanged, this, [this](int row) {
        slider->setValue(row);
    });
    setIni();
}

void EventRecord::addEventObj(QLineEdit *obj, QObject *parent, const QString &objname)
{
    if (!obj || !parent) {
        return ;
    }

    obj->setObjectName(objname);
    appendEventWidgets(obj);
    connect(obj, &QLineEdit::textChanged, parent, [obj] (const QString &text){
        QString data = decodeEventObjValue(obj, text);
        if (data.size()) {
            writeLog(data);
        }
    });
}

QString EventRecord::decodeEventObjValue(QLineEdit *obj, const QString &value)
{
    if (!obj) {
        return "";
    }

    qDebug() << obj->metaObject()->className() << obj->objectName() << value;
    QJsonObject operObj {
        {"Time", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")},
        {"className", obj->metaObject()->className()},
        {"objectName", obj->objectName()},
        {"value", value}
    };
    QJsonDocument jsonDoc;

    jsonDoc.setObject(operObj);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

void EventRecord::addEventObj(QPushButton *obj, QObject *parent, const QString &objname)
{
    if (!obj || !parent) {
        return ;
    }

    obj->setObjectName(objname);
    appendEventWidgets(obj);
    connect(obj, &QPushButton::clicked, parent, [obj] (){
        QString data = decodeEventObjValue(obj);
        if (data.size()) {
            writeLog(data);
        }
    });
}

QString EventRecord::decodeEventObjValue(QPushButton *obj)
{
    if (!obj) {
        return "";
    }

    qDebug() << obj->metaObject()->className() << obj->objectName() << "clicked";
    QJsonObject operObj {
        {"Time", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")},
        {"className", obj->metaObject()->className()},
        {"objectName", obj->objectName()},
        {"value", "Clicked"}
    };
    QJsonDocument jsonDoc;

    jsonDoc.setObject(operObj);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

void EventRecord::addEventObj(QCheckBox *obj, QObject *parent, const QString &objname)
{
    if (!obj || !parent) {
        return ;
    }

    obj->setObjectName(objname);
    appendEventWidgets(obj);
    connect(obj, &QCheckBox::stateChanged, parent, [obj] (int state){
        QString data = decodeEventObjValue(obj, state);
        if (data.size()) {
            writeLog(data);
        }
    });
}

QString EventRecord::decodeEventObjValue(QCheckBox *obj, int state)
{
    if (!obj) {
        return "";
    }

    qDebug() << obj->metaObject()->className() << obj->objectName() << state;
    QString operDesc = state ? "Checked" : "Unchecked";
    QJsonObject operObj {
        {"Time", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")},
        {"className", obj->metaObject()->className()},
        {"objectName", obj->objectName()},
        {"value", operDesc}
    };
    QJsonDocument jsonDoc;

    jsonDoc.setObject(operObj);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

void EventRecord::addEventObj(QComboBox *obj, QObject *parent, const QString &objname)
{
    if (!obj) {
        return ;
    }

    obj->setObjectName(objname);
    appendEventWidgets(obj);
    if (parent) {
        connect(obj, &QComboBox::currentTextChanged, parent, [obj] (const QString &text){
            addEventObjValue(obj, text);
        });
    }
}

QString EventRecord::decodeEventObjValue(QComboBox *obj, const QString &value)
{
    if (!obj) {
        return "";
    }

    qDebug() << obj->metaObject()->className() << obj->objectName() << value;
    QJsonObject operObj {
        {"Time", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")},
        {"className", obj->metaObject()->className()},
        {"objectName", obj->objectName()},
        {"value", value}
    };
    QJsonDocument jsonDoc;

    jsonDoc.setObject(operObj);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

void EventRecord::addEventObjValue(QComboBox *obj, const QString &value)
{
    QString data = decodeEventObjValue(obj, value);
    if (data.size()) {
        writeLog(data);
    }
}

bool EventRecord::readEventLog()
{
    QStringList logfile;

    return readLogLines(logfile, 10);
}

void EventRecord::showWidget()
{
    EventRecord::runningFlag = true;

    if (!this->isHidden()) {
        this->close();
    }

    QStringList logfile;
    int cnt = displayCount->text().toInt();
    if (cnt > 30000) {
        cnt = 30000;
        displayCount->setText("30000");
    }

    readLogLines(logfile, cnt);
    recordList->clear();
    for (int i = logfile.size(); i ; i--) {
        addRecordItem(logfile.at(i - 1));
    }
    recordList->verticalScrollBar()->setMinimum(0);
    recordList->verticalScrollBar()->setMaximum(recordList->count() - 1);
    slider->setMinimum(0);
    slider->setMaximum(recordList->count() - 1);
    recordList->setCurrentRow(0);
    setFoucusStyle(true);
    this->show();
    hangUp();
    setIni();
}

bool EventRecord::addRecordItem(const QString &data)
{
    QJsonParseError err;

    QJsonObject dataObj = QJsonDocument::fromJson(QByteArray(data.toUtf8()), &err).object();
    if (err.error)
        return false;

    QString objectName = dataObj["objectName"].toString();
    QString className = dataObj["className"].toString();
    QString Time = dataObj["Time"].toString();
    QString value = dataObj["value"].toString();

    QListWidgetItem *item = new QListWidgetItem();

    if (className == "QComboBox") {
        if (objectName == EventRecord::sensorSelectObjName) {
            qDebug() << EventRecord::sensorSelectObjName;
            QFont font = item->font();
            font.setBold(true);
            font.setPointSize(font.pointSize() + 2);
            item->setFont(font);
        }
        else if (objectName == EventRecord::presetSelectObjName) {
            qDebug() << EventRecord::presetSelectObjName;
            QFont font = item->font();
            font.setBold(true);
            font.setPointSize(font.pointSize() + 1);
            item->setFont(font);
        }
        // item->setIcon(QIcon(":/icon/combox.png"));
    }
    else if (className == "QLineEdit") {
        // item->setIcon(QIcon(":/icon/lineedit.png"));
    }
    else if (className == "QPushButton") {
        // item->setIcon(QIcon(":/icon/pushbutton.png"));
    }
    else if (className == "QCheckBox") {
        // item->setIcon(QIcon(":/icon/checkbox.png"));
    }

    item->setText(QString("[%1]{%2} %3 【%4】")\
                  .arg(recordList->count() + 1).arg(Time, objectName, value));
    recordList->addItem(item);
    item->setToolTip(QJsonDocument(dataObj).toJson(QJsonDocument::Compact));

    return true;
}

void EventRecord::play()
{
    QListWidgetItem *item = recordList->item(recordList->currentRow());
    if (!item) {
        return ;
    }
    parse(item->toolTip());
}

void EventRecord::stopPlay()
{
    playTimer->stop();
    playBtn->setIcon(QIcon(":/icon/start.png"));
    setFoucusStyle(false);
}

void EventRecord::next()
{
    qDebug() << "next slider value" << slider->value();
    if (slider->value() > slider->minimum()) {
        slider->setValue(slider->value() - 1);
        qDebug() << "next slider value" << slider->value();
    }
}

void EventRecord::prev()
{
    qDebug() << "prev slider value" << slider->value();
    if (slider->value() < slider->maximum()) {
        slider->setValue(slider->value() + 1);
        qDebug() << "prev slider value" << slider->value();
    }
}

void EventRecord::closeEvent(QCloseEvent *event)
{
    if (!event) {
        qDebug() << "event is nullptr";
    }

    stopPlay();
    recordList->clear();
    continued();
    SetOption::setValue("recallWidget", "height", this->size().height());
    SetOption::setValue("recallWidget", "width", this->size().width());
    SetOption::setValue("recallWidget", "displayMaxnum", displayCount->text());
    SetOption::setValue("recallWidget", "playSpeed", playSpeed->currentText());
    EventRecord::runningFlag = false;
}
#include <QGuiApplication>

void EventRecord::setIni()
{
    QList<QScreen *> screens = QGuiApplication::screens();
    QRect rect = screens.at(0)->geometry();

    int width = SetOption::value("recallWidget", "width").toInt();
    int height = SetOption::value("recallWidget", "height").toInt();
    width = width > rect.width() ? rect.width() : width;
    height = height > rect.height() ? rect.height() : height;

    this->resize(width, height);
    displayCount->setText(SetOption::value("recallWidget", "displayMaxnum").toString());
    playSpeed->setCurrentText(SetOption::value("recallWidget", "playSpeed").toString());
}

void EventRecord::setFoucusStyle(bool flag)
{
    QMap<QString, QWidget *>::const_iterator\
            wlist = eventObjects.constBegin();

    while (wlist != eventObjects.constEnd()) {
        QWidget *w = wlist.value();

        QString className = w->metaObject()->className();

        if (flag) {
            if (className == "QComboBox") {
                QComboBox *combox = (QComboBox *)w;
                combox->setStyleSheet("QComboBox:focus{\
                                        background-color: #FFF68F;\
                                        }");
            }
            else if (className ==  "QLineEdit") {
                QLineEdit *lineEdit = (QLineEdit *)w;
                lineEdit->setStyleSheet("QLineEdit:focus{\
                                        background-color: #FFF68F;\
                                        }");
            }
            else if (className == "QPushButton") {

            }
            else if (className == "QCheckBox") {
                QCheckBox *checkBox = (QCheckBox *)w;
                checkBox->setStyleSheet("QCheckBox:focus{\
                                        background-color: #FFF68F;\
                                        }");
            }
        }
        else {
            if (className == "QComboBox") {
                QComboBox *combox = (QComboBox *)w;
                combox->setStyleSheet("QComboBox:focus{\
                                        background-color: #FFFFFF;\
                                        }");
            }
            else if (className ==  "QLineEdit") {
                QLineEdit *lineEdit = (QLineEdit *)w;
                lineEdit->setStyleSheet("QLineEdit:focus{\
                                        background-color: #FFFFFF;\
                                        }");
            }
            else if (className == "QPushButton") {

            }
            else if (className == "QCheckBox") {
                QCheckBox *checkBox = (QCheckBox *)w;
                checkBox->setStyleSheet("QCheckBox:focus{\
                                        background-color: #FFFFFF;\
                                        }");
            }
        }

        wlist++;
        }
}

bool EventRecord::parse(const QString &data)
{
    QJsonParseError err;

    QJsonObject dataObj = QJsonDocument::fromJson(QByteArray(data.toUtf8()), &err).object();
    if (err.error) {

        return false;
    }

    QString objectName = dataObj["objectName"].toString();
    QString className = dataObj["className"].toString();
    //QString Time = dataObj["Time"].toString();
    QString value = dataObj["value"].toString();

    if (!eventObjects.contains(objectName)) {
        return false;
    }
    QWidget *w = eventObjects[objectName];

    w->setFocus();
    if (className == "QComboBox") {
        QComboBox *combox = (QComboBox *)w;
        combox->setCurrentText(value);
    }
    else if (className == "QLineEdit") {
        QLineEdit *lineEdit = (QLineEdit *)w;
        lineEdit->setText(value);
    }
    else if (className == "QPushButton") {

    }
    else if (className == "QCheckBox") {
        QCheckBox *checkBox = (QCheckBox *)w;

        if (value == "Checked") {
            checkBox->setCheckState(Qt::CheckState::Checked);
        }
        else if (value == "Unchecked"){
            checkBox->setCheckState(Qt::CheckState::Unchecked);
        }
    }

    return true;
}

void EventRecord::hangUp()
{
    QMap<QString, QWidget *>::const_iterator\
            wlist = eventObjects.constBegin();

    while (wlist != eventObjects.constEnd()) {
        QWidget *w = wlist.value();
        QString className = w->metaObject()->className();

        if (className == "QComboBox") {
            QComboBox *combox = (QComboBox *)w;
            if (combox->objectName() == EventRecord::sensorSelectObjName) {
                hangUpData.insert(0, decodeEventObjValue(combox, combox->currentText()));
            }
            else if (combox->objectName() == EventRecord::presetSelectObjName) {
                hangUpData.insert(1, decodeEventObjValue(combox, combox->currentText()));
            }
            else {
                hangUpData.append(decodeEventObjValue(combox, combox->currentText()));
            }
        }
        else if (className ==  "QLineEdit") {
            QLineEdit *lineEdit = (QLineEdit *)w;
            hangUpData.append(decodeEventObjValue(lineEdit, lineEdit->text()));
        }
        else if (className == "QPushButton") {

        }
        else if (className == "QCheckBox") {
            QCheckBox *checkBox = (QCheckBox *)w;
            hangUpData.append(decodeEventObjValue(checkBox, checkBox->checkState()));
        }
        wlist++;
    }
}

void EventRecord::continued()
{
    for (int i = 0; i < hangUpData.size(); i++) {
        parse(hangUpData.at(i));
    }
    hangUpData.clear();
}

QString EventRecord::objName(const QString &prefix, const QString &name)
{
    return QString("%1_%2").arg(prefix, name);
}




