#ifndef STEPTIPS_H
#define STEPTIPS_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QQueue>

class StepTips : public QObject
{
    Q_OBJECT
public:
    explicit StepTips(QObject *parent = nullptr);
    ~StepTips();
    void addStep(QWidget *widget);
    void addStep(QWidget *widget, const QString &originalText);
    void addStep(QWidget *widget, const QString &originalText, const QString &text);
    void addStep(QWidget *widget, const QString &originalText, const QString &text, const QString &tips);
    void clear();
    void reset();
    void next();
    void prev();
    void showTips();
    void hideTips();
    bool isfirst();
    bool islast();
    void highlightWidget(int index);
    void highlightWidget(QWidget *widget);
    void highlightCancel();
    QWidget *currTipsWidget();
private:
    struct tipsWidget {
        QWidget *widget;
        QString originalText;
        QString newText;
        QString tips;
    };
    int currHighlight = -1;
    int currIndex = 0;
    QQueue<tipsWidget *> stepQueue;
signals:

};

#endif // STEPTIPS_H
