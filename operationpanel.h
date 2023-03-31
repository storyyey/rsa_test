#ifndef OPERATIONPANEL_H
#define OPERATIONPANEL_H

#include <QWidget>

class OperationPanel : public QWidget
{
    Q_OBJECT
public:
    enum {
        activeConfig,
        activeAutoConfig,
        activePreview,
        activeRecall,
        activeSaveas,
        activeDebug
    };

    explicit OperationPanel(QWidget *parent = nullptr);

signals:
    void active(int type);
};

#endif // OPERATIONPANEL_H
