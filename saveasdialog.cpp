#include "saveasdialog.h"
#include <QLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QLabel>

SaveAsDialog::SaveAsDialog()
{
    QString style =\
            "QLineEdit {\
                border:1px solid #63B8FF;\
                border-radius:4px;\
                height: 30px;\
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
    style += "QListWidget {\
                border: 0px;\
                background-color: #EDEDED;\
             }"\
            "QScrollBar {\
                width: 0px;\
                height: 0px;\
            }"\
            "QListWidget::item {\
                background-color: #545454; \
                border-radius: 8px;\
                color: #FFFFFF;\
                height: 40px;\
                border: 1px solid #FCFCFC;\
            }";

    this->setStyleSheet(style);
    QVBoxLayout *mainLyaout = new QVBoxLayout();
    this->setLayout(mainLyaout);

    QDesktopWidget dw;
    resize(QSize(dw.width() / 4, dw.height() / 2));
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#FF0000"));
    QLabel *helpLabel = new QLabel("※输入方案名点击按钮后将保存为一个新的方案配置，\n"\
                                   "直接点击下列已存在方案名时将覆盖旧方案配置。");
    helpLabel->setPalette(pa);
    mainLyaout->addWidget(helpLabel);
    QHBoxLayout *proEditLayout = new QHBoxLayout();
    newProBtn = new QPushButton("存为新方案");
    newProBtn->setIcon(QIcon(":/icon/filesave.png"));
    proEditLayout->addWidget(newProBtn);
    proNameEdit = new QLineEdit();
    proNameEdit->setPlaceholderText("新配置方案名(不能与已有方案同名)");
    proEditLayout->addWidget(proNameEdit);
    mainLyaout->addLayout(proEditLayout);
    connect(newProBtn, SIGNAL(clicked()), this, SLOT(saveProject()));

    proList = new QListWidget();
    mainLyaout->addWidget(proList);
    connect(proList, SIGNAL(itemClicked(QListWidgetItem *)),\
            this, SLOT(listWidgetItemClickd(QListWidgetItem *)));
}

SaveAsDialog::~SaveAsDialog()
{
    qDebug() << "~SaveAsDialog";
}

void SaveAsDialog::setProjectList(QStringList &pl)
{
    proNameEdit->clear();
    proList->clear();
    proNameList.clear();
    proNameList = pl;
    proList->addItems(proNameList);
}

void SaveAsDialog::listWidgetItemClickd(QListWidgetItem *item)
{
    switch (QMessageBox::information(NULL, tr("另存为方案"), \
            QString("是否覆盖(%1)方案?").arg(item->text()), tr("是"), tr("否"), 0, 1 ))
    {
      case 0:
           emit saveAs(item->text());
           QMessageBox::information(NULL, tr("方案预设"), tr("保存成功!"), tr("是"));
           this->close();
           break;
      case 1:
      default:
            qDebug() << "否";
          break;
    }
}

void SaveAsDialog::saveProject()
{
    if (!proNameEdit->text().size()) {
        QMessageBox::warning(NULL, tr("方案预设"), tr("方案名不能为空!"), tr("是"));
        return ;
    }
    QString proname = proNameEdit->text();
    if (proNameList.contains(proname)) {
        QMessageBox::warning(NULL, tr("方案预设"), tr("存在同名方案!"), tr("是"));
        return ;
    }
    emit saveAs(proNameEdit->text());
    QMessageBox::information(NULL, tr("方案预设"), tr("保存成功!"), tr("是"));
    this->close();
}
