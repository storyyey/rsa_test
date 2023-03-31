#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>

class HelpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpWidget(QWidget *parent = nullptr);
    ~HelpWidget();
    void help();
signals:

};

#endif // HELPWIDGET_H
