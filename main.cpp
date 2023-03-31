#include "mainwindow.h"
#include "onlyapp.h"
#include <QApplication>
#include <settingOption.h>

static void mainWindowSetini(MainWindow &w)
{
    int high = SetOption::value("MainWindow", "height").toInt();
    int width = SetOption::value("MainWindow", "width").toInt();
    if (high > 0 && width > 0) {
        w.resize(width, high);
    }
}
static void applicationSetini(QApplication &a)
{
    QString family = SetOption::value("Application", "font_family").toString();
    int size = SetOption::value("Application", "font_size").toInt();

    if (family.size() && size > 0) {
        a.setFont(QFont(family, size));
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    OnlyApp oa;

    if (!oa.isOnly()) {
        QMessageBox::warning(NULL, "提示", "传感器配置工具已经在运行!",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return 0;
    }
    applicationSetini(a);
    a.setWindowIcon(QIcon(":/icon/progress.png"));
    w.setWindowTitle("传感器配置工具");
    w.show();
    mainWindowSetini(w);

    int exitcode = a.exec();
    if (exitcode == MainWindow::REBOOT) {
        QProcess::startDetached(a.applicationFilePath(), QStringList());
    }
    SetOption::setValue("MainWindow", "height", w.size().height());
    SetOption::setValue("MainWindow", "width", w.size().width());

    return 0;
}
