#include "debugwidget.h"
#include "settingOption.h"
#include <QFontDialog>
#include <QDebug>

DebugWidget::DebugWidget(QWidget *parent) : QWidget(parent)
{
    QString comBoxstyle = "QComboBox {\
                        border:1px solid #63B8FF;\
                        border-radius:4px;\
                        height: 30px;\
                    }";
    QString Style = ".QWidget {\
                background-color: #FAFAFA;\
                border:2px solid #63B8FF;\
                border-radius: 10px;\
             }";
    Style += "QListWidget {\
                   border: 0px;\
                   outline:0px;\
                   background-color: #EDEDED;\
                }"\
               "QListWidget::item {\
                   border-radius: 8px;\
                   height: 30px;\
                   border: 2px solid #FFFFFF;\
               }";
    Style += "QPlainTextEdit {\
                border: 2px solid #FFFFFF;\
                border-radius: 8px;\
             }";
    QString btnStyle = "QPushButton:hover {\
                background-color: #EBEBEB;\
             }"\
             "QPushButton:pressed {\
                background-color: #F8F8FF;\
             }";
    btnStyle += "QPushButton {\
                background-color: #FAFAFA;\
                border-radius: 10px;\
                height: 35px;\
              }";

    this->setStyleSheet(btnStyle + comBoxstyle + Style);
    QVBoxLayout *mLayout = new QVBoxLayout();
    this->setLayout(mLayout);
    mLayout->setContentsMargins(5, 5, 5, 5);

    QHBoxLayout *ssLayout = new QHBoxLayout();
    mLayout->addLayout(ssLayout);
    cmdBox = new QComboBox();
    ssLayout->addWidget(cmdBox);

    QPushButton *fontsizeBtn = new QPushButton("字体设置");
    fontsizeBtn->setMaximumWidth(120);
    connect(fontsizeBtn, SIGNAL(clicked()), this, SLOT(fontSizeChange()));
    ssLayout->addWidget(fontsizeBtn);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    mLayout->addLayout(inputLayout);
    inputCombox = new QComboBox();
    inputCombox->setEditable(true);
    inputDbgText = inputCombox->lineEdit();
    inputLayout->addWidget(inputCombox);

    sendTextButton = new QPushButton(this);
    sendTextButton->setMaximumWidth(120);
    sendTextButton->setText("发送数据");
    sendTextButton->setIcon(QIcon(":/icon/send.png"));
    inputLayout->addWidget(sendTextButton);

    QHBoxLayout *soLayout = new QHBoxLayout();
    mLayout->addLayout(soLayout);

    outputDbgText = new QPlainTextEdit(this);
    outputDbgText->setMaximumBlockCount(12000);
    soLayout->addWidget(outputDbgText);

    shortcutCmd = new QListWidget();
    shortcutCmd->setMaximumWidth(140);
    soLayout->addWidget(shortcutCmd);
    connect(shortcutCmd, &QListWidget::itemClicked,\
            this, [this](QListWidgetItem *item){
        inputDbgText->setText(item->toolTip());
        sendTextButton->click();
    });
    addShortcutAt("进入AT", "AT+CMD=1");
    addShortcutAt("命令支持", "AT+?");
    addShortcutAt("查询配置", "AT+CONF?");
    addShortcutAt("保存配置", "AT+INI=255");
    addShortcutAt("退出AT", "AT+CMD=0");

    connect(sendTextButton, SIGNAL(clicked()), this, SLOT(sendTextSlot()));
    connect(cmdBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectCmdBox(QString)));

    setini();
}

DebugWidget::~DebugWidget()
{
    qDebug() << "~DebugWidget()";
}

void DebugWidget::print(const QString &str)
{
    static uint32_t lineTotal = 0;

    QDateTime currDate = QDateTime::currentDateTime();
    QString currDateStr = currDate.toString("hh:mm:ss");

    outputDbgText->appendPlainText(QString("%1 |%2【%3】>%4").arg(++lineTotal).arg(currDateStr).arg(str.size()).arg(str));
}

void DebugWidget::appendCmdBox(const QStringList &list)
{
    cmdBox->addItems(list);
}

void DebugWidget::addShortcutAt(const QString &name, const QString &cmd)
{
    QListWidgetItem *item = new QListWidgetItem(name);
    item->setToolTip(cmd);

    shortcutCmd->addItem(item);
}

void DebugWidget::setini()
{
    QFont font(SetOption::value(iniMainGroup, "font_family").toString(),\
               SetOption::value(iniMainGroup, "font_size").toInt());

    outputDbgText->setFont(font);
}

void DebugWidget::sendTextSlot()
{
    bool flag = true;

    if (!inputDbgText->text().size()) {
        return ;
    }

    for (int n = 0; n < inputCombox->count(); n++) {
        if (inputCombox->itemText(n) == inputDbgText->text()) {
            flag = false;
        }
    }
    if (flag) {
        inputCombox->addItem(inputDbgText->text());
    }
    emit sendMessage(inputDbgText->text() + "\r\n");
}

void DebugWidget::selectCmdBox(const QString &text)
{
    inputDbgText->setText(text);
}

void DebugWidget::fontSizeChange()
{
    bool ok;

    QFont font = QFontDialog::getFont(&ok);
    if (ok) {
        outputDbgText->setFont(font);
        SetOption::setValue(iniMainGroup, "font_family", font.family());
        SetOption::setValue(iniMainGroup, "font_size", font.pointSize());
    }
}

