#ifndef RIBBONBARWIDGET_H
#define RIBBONBARWIDGET_H

#include <QWidget>

class QEvent;
class QMenuBar;
class QStackedWidget;
class QToolButton;

class RibbonBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RibbonBarWidget(QWidget *parent = nullptr);

    void bindMenuBar(QMenuBar *menuBar);
    void setProjectPanelVisible(bool visible);

signals:
    void projectPanelToggled(bool visible);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QStackedWidget *ribbonStack = nullptr;
    QMenuBar *boundMenuBar = nullptr;
    QToolButton *projectPanelButton = nullptr;

    QWidget* createPage(const QStringList& actions);
    QToolButton* createButton(const QString& text);
};

#endif // RIBBONBARWIDGET_H
