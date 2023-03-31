#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTime>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>

class DebugWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DebugWidget(QWidget *parent = nullptr);
    ~DebugWidget();
    void print(const QString &str);
    void appendCmdBox(const QStringList &list);
    void addShortcutAt(const QString &name, const QString &cmd);
    void setini();
private:
    QString iniMainGroup = "DebugWidget";
    QPlainTextEdit *outputDbgText;
    QComboBox *inputCombox;
    QLineEdit *inputDbgText;
    QPushButton *sendTextButton;
    QComboBox *cmdBox;
    QListWidget *shortcutCmd;
signals:
    void sendMessage(const QString &msg);
private slots:
    void sendTextSlot();
    void selectCmdBox(const QString &text);
    void fontSizeChange();
};

#endif // DEBUGWIDGET_H
