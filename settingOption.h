#ifndef SETTINGOPTION_H
#define SETTINGOPTION_H

#include <QSettings>
#include <QTextCodec>

class SetOption {
public:
    static void setValue(const QString &group, const QString &key, const QVariant &value) {
        QSettings settings("sensorTool.ini", QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("utf-8"));

        settings.beginGroup(group);
        settings.setValue(key, value);
        settings.endGroup();
    }
    static QVariant value(const QString &group, const QString &key) {
        QSettings settings("sensorTool.ini", QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("utf-8"));

        return settings.value(group + "/" + key);
    }
};


#endif // SETTINGOPTION_H
