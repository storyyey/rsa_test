#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include "uartwidget.h"
#include "debugwidget.h"
#include "sensorconfigwidget.h"
#include "tableswidget.h"
#include "helpwidget.h"

extern DebugWidget *g_debugWidget;
inline void windowConsole(const QString &str)
{
    if (g_debugWidget) {
        g_debugWidget->print(str);
    }
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void reboot();
    static int const REBOOT = 65785;
private:
    UartWidget *uartWidget;
    SensorConfigWidget *sensorConfigWidget;
    ATCmd *ATcmd;
    PromptDialog *configProgressDialog;
    HelpWidget *helpWidget;
    DataBase *parentDB;
    EventRecord *eventRecord;
private slots:
    void helpMenuConfigAction();
};
#endif // MAINWINDOW_H
