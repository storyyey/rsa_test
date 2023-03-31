#ifndef SAVEASDIALOG_H
#define SAVEASDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QStringList>
#include <QPushButton>
#include <QLineEdit>

class SaveAsDialog : public QDialog
{
    Q_OBJECT
public:
    SaveAsDialog();
    ~SaveAsDialog();
    void setProjectList(QStringList &pl);
private:
    QListWidget *proList;
    QPushButton *newProBtn;
    QLineEdit *proNameEdit;
    QStringList proNameList;
private slots:
    void listWidgetItemClickd(QListWidgetItem *item);
    void saveProject();
signals:
    void saveAs(const QString &name);
};

#endif // SAVEASDIALOG_H
