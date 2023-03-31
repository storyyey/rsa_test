#include "mainwindow.h"
#include <QMenuBar>
#include <QFontDialog>
#include <settingOption.h>
#include "operationpanel.h"

DebugWidget *g_debugWidget;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setStyleSheet("QMainWindow{background-color: #4D4D4D;}");
    g_debugWidget = new DebugWidget();
    TablesWidget *tables = new TablesWidget();
    setCentralWidget(tables);
    parentDB = new DataBase(this);

    QDockWidget *debugDock = new QDockWidget("调试窗口", this);
    connect(debugDock, &QDockWidget::dockLocationChanged,\
            this, [debugDock](Qt::DockWidgetArea area) {

        if (Qt::NoDockWidgetArea == area) {
            debugDock->resize(QSize(1000, 600));
        }
    });
    debugDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    debugDock->setWidget(g_debugWidget);
    debugDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, debugDock);
    debugDock->close();

    configProgressDialog = new PromptDialog();
    QWidget *operWidget = new QWidget();
    QVBoxLayout *operLayout = new QVBoxLayout();
    operLayout->setContentsMargins(0, 0, 0, 0);
    operWidget->setLayout(operLayout);
    uartWidget = new UartWidget();
    uartWidget->setConfigProgressDialog(configProgressDialog);
    operLayout->addWidget(uartWidget);

    OperationPanel *operationPanel = new OperationPanel();
    operLayout->addWidget(operationPanel);

    sensorConfigWidget = new SensorConfigWidget(tables, parentDB);
    sensorConfigWidget->setPromptDialog(configProgressDialog);
    operLayout->addWidget(sensorConfigWidget);;
    QDockWidget *actionDock = new QDockWidget("操作", this);
    actionDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    actionDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    actionDock->setWidget(operWidget);
    QWidget* lTitleBar = actionDock->titleBarWidget();
    QLabel *actionBar = new QLabel();
    actionDock->setTitleBarWidget(actionBar);
    delete lTitleBar;
    addDockWidget(Qt::LeftDockWidgetArea, actionDock);
    connect(uartWidget, SIGNAL(barcodeData(QString)), sensorConfigWidget, SLOT(newDevicesInfo(QString)));
    connect(g_debugWidget, SIGNAL(sendMessage(QString)), uartWidget, SLOT(sendToSensor(QString)));
    connect(operationPanel, &OperationPanel::active, sensorConfigWidget, &SensorConfigWidget::activeSlot);

    ATcmd = new ATCmd(this);
    uartWidget->setATcmd(ATcmd);
    sensorConfigWidget->setATcmd(ATcmd);
    g_debugWidget->appendCmdBox(ATcmd->getCmdLst());

    QMenuBar *menuBr = menuBar();
    menuBr->setStyleSheet(".QMenuBar{background-color: rgb(220,220,220);}");
    QMenu *fileMenu = menuBr->addMenu("文件(F)");
    QAction *resetConfigRecord = fileMenu->addAction("重置配置记录");
    resetConfigRecord->setIcon(QIcon(":/icon/tablereset.png"));
    connect(resetConfigRecord, &QAction::triggered, this, [this, tables]{
        if (parentDB->resetConfigRecord()) {
            tables->resetTable();
        }
    });

    QMenu *tools = menuBr->addMenu("工具(T)");
    QMenu *helpFile = menuBr->addMenu("帮助(H)");
    QAction *startAction = helpFile->addAction("使用帮助文档");
    startAction->setIcon(QIcon(":/icon/help.png"));
    startAction->setShortcut(tr("ctrl+h"));
    setMenuBar(menuBr);
    helpWidget = new HelpWidget();
    helpWidget->hide();
    connect(startAction, SIGNAL(triggered()), this, SLOT(helpMenuConfigAction()));

    QAction *showDebugAction = tools->addAction("调试窗口");
    showDebugAction->setIcon(QIcon(":/icon/debug.png"));
    showDebugAction->setShortcut(tr("ctrl+d"));
    connect(showDebugAction, &QAction::triggered, this, [debugDock] {
        debugDock->isHidden() ? debugDock->show(), true : debugDock->close();
    });
    eventRecord = new EventRecord();
    connect(operationPanel, &OperationPanel::active, this, [this, debugDock] (int type) {
        switch (type) {
            case OperationPanel::activeDebug:
                debugDock->isHidden() ? debugDock->show(), true : debugDock->close();
                break;
            case OperationPanel::activeRecall:
                eventRecord->showWidget();
                break;
            default:
                break;
        }
    });

    QAction *fontStyle = tools->addAction("字体风格");
    fontStyle->setIcon(QIcon(":/icon/font.png"));
    connect(fontStyle, &QAction::triggered, this, [this]{
        bool ok;

        QFont font = QFontDialog::getFont(&ok);
        if (ok) {
            SetOption::setValue("Application", "font_family", font.family());
            SetOption::setValue("Application", "font_size", font.pointSize());

            reboot();
        }
    });
}

MainWindow::~MainWindow()
{
    configProgressDialog->deleteLater();
    helpWidget->deleteLater();
    eventRecord->deleteLater();
}

void MainWindow::helpMenuConfigAction()
{
    helpWidget->show();
    helpWidget->help();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   switch (QMessageBox::information(this, tr("退出传感器配置工具"), tr("确认退出?"), tr("是"), tr("否"), 0, 1 ))
   {
     case 0:
          event->accept();
          break;
     case 1:
     default:
         event->ignore();
         break;
   }
}

void MainWindow::reboot()
{
    QCoreApplication::exit(REBOOT);
}
