#ifndef LOGPANELWIDGET_H
#define LOGPANELWIDGET_H

#include <QTableWidget>

class LogPanelWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit LogPanelWidget(QWidget *parent = nullptr);
};

#endif // LOGPANELWIDGET_H
