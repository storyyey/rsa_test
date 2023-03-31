#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QListWidget>
#include <QPropertyAnimation>
#include "sensordata.h"

class PromptDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PromptDialog();
    ~PromptDialog();
    void setDevAddr(const QString &text);
    void setoperationPrompts(const QString &text, QFont *ft = nullptr, QPalette *pa = nullptr);
    void setWarning(const QString &text);
    void setInfomation(const QString &text);
    void setError(const QString &text);
    void setSuccess(const QString &text);
    void showPrompt();
    void setMode(const QString &text);
    void setMovieSpeed(int p);
    void addResultItem(const QString &text);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void setSensorType(SensorData::sensorType sensorType);
private:
    QHBoxLayout *manualWidgetLayout;
    QMovie *movie;
    QLabel *devAddrLabel;
    QLabel *operationPromptsLabel;
    QLabel *movieLabel;
    QPushButton *modeLabel;
    QPushButton *exitBtn;
    QPushButton *clearBtn;
    QListWidget *resultRecord;
    QLabel *resultTotalLabel;
    QLabel *helpInfoLabel;
    QLabel *msgIconlabel;
signals:
    void cannel();
private slots:
    void closePrompt();
    void clearResultRecord();
};

#endif // PROMPTDIALOG_H
