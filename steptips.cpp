#include "steptips.h"
#include <QDebug>

StepTips::StepTips(QObject *parent) : QObject(parent)
{

}

StepTips::~StepTips()
{
    qDebug() << "~StepTips()";
    for (int index = 0; index < stepQueue.count(); index++) {
        delete stepQueue.at(index);
    }
}

void StepTips::addStep(QWidget *widget)
{
    if (widget == nullptr) return ;
    tipsWidget *tw = new tipsWidget();
    tw->widget = widget;
    stepQueue.append(tw);
    highlightWidget(currIndex);
}

void StepTips::addStep(QWidget *widget, const QString &originalText)
{
    if (widget == nullptr) return ;
    addStep(widget);
    tipsWidget *tw = stepQueue.at(stepQueue.count() - 1);
    tw->originalText = originalText;
}

void StepTips::addStep(QWidget *widget, const QString &originalText, const QString &newText)
{
    if (widget == nullptr) return ;
    addStep(widget, originalText);
    tipsWidget *tw = stepQueue.at(stepQueue.count() - 1);
    tw->newText = newText;
}

void StepTips::addStep(QWidget *widget, const QString &originalText, const QString &newText, const QString &tips)
{
    if (widget == nullptr) return ;
    addStep(widget, originalText, newText);
    tipsWidget *tw = stepQueue.at(stepQueue.count() - 1);
    tw->tips = tips;
}

void StepTips::clear()
{
    highlightCancel();
    while (!stepQueue.isEmpty()) {
        tipsWidget *tw = stepQueue.takeFirst();
        delete tw;
    }
}

void StepTips::reset()
{
    currIndex = 0;
    highlightWidget(currIndex);
}

void StepTips::next()
{
    if (!islast()) {
        currIndex++;
    }
    else {
       currIndex = 0;
    }
    highlightWidget(currIndex);
}

void StepTips::prev()
{
    if (!isfirst()) {
        currIndex--;
    }
    else {
        currIndex = stepQueue.count() - 1;
    }
    highlightWidget(currIndex);
}

void StepTips::showTips()
{
    if (stepQueue.isEmpty()) {
        return ;
    }

    tipsWidget *tw = stepQueue.at(currHighlight);
    if (tw && tw->originalText.size()) {
        QPushButton *btn = qobject_cast<QPushButton*>(tw->widget);
        if (tw->newText.size()) {
            btn->setText(tw->newText);
        }
        btn->setToolTip(tw->tips);
    }
}

void StepTips::hideTips()
{
    if (stepQueue.isEmpty()) {
        return ;
    }

    tipsWidget *tw = stepQueue.at(currHighlight);
    if (tw && tw->originalText.size()) {
        QPushButton *btn = qobject_cast<QPushButton*>(tw->widget);
        btn->setText(tw->originalText);
        btn->setToolTip("");
    }
}

bool StepTips::isfirst()
{
    if (currIndex <= 0) {
        currIndex = 0;
        return true;
    }
    return false;
}

bool StepTips::islast()
{
    if (currIndex >= stepQueue.count() - 1) {
        currIndex = stepQueue.count() - 1;
        return true;
    }
    return false;
}

void StepTips::highlightWidget(int index)
{
    if (stepQueue.isEmpty() ||\
        index > stepQueue.count() - 1 ||\
        index < 0) {
        return ;
    }

    highlightCancel();
    tipsWidget *tw = stepQueue.at(index);
    if (tw) {
        if (QString(tw->widget->metaObject()->className()) == "QPushButton") {
            QString style = "QPushButton:hover {\
                                background-color: #EBEBEB;\
                                border: 3px solid #DEDEDE;\
                             }"\
                            "QPushButton:pressed {\
                                background-color: #F8F8FF;\
                                border: 3px solid #EBEBEB;\
                            }";
                    style += "QPushButton {\
                                background-color: #EBEBEB;\
                                border: 3px solid #C7C7C7;\
                                border-radius: 6px;\
                                height: 30px;\
                             }";
            tw->widget->setStyleSheet(style);
        }
        currHighlight = index;
    }
}

void StepTips::highlightWidget(QWidget *widget)
{
    if (widget == nullptr) {
        return ;
    }

    for (int index = 0; index < stepQueue.count(); index++) {
        if (stepQueue.at(index)->widget == widget) {
            currIndex = index;
            highlightWidget(currIndex);
            return ;
        }
    }
}

void StepTips::highlightCancel()
{
    if (stepQueue.isEmpty() ||\
        currHighlight > stepQueue.count() - 1 ||\
        currHighlight < 0) {
        return ;
    }
    hideTips();
    tipsWidget *tw = stepQueue.at(currHighlight);
    if (tw) {
        if (QString(tw->widget->metaObject()->className()) == "QPushButton") {
            QString style = "QPushButton:hover {\
                                background-color: #EBEBEB;\
                                border: 3px solid #DEDEDE;\
                             }"\
                            "QPushButton:pressed {\
                                background-color: #F8F8FF;\
                                border: 3px solid #EBEBEB;\
                            }";
                    style += "QPushButton {\
                                background-color: #AAAAAA;\
                                border: 3px solid #BDBDBD;\
                                border-radius: 6px;\
                                height: 30px;\
                             }";
            tw->widget->setStyleSheet(style);
        }
        currHighlight = -1;
    }
}
