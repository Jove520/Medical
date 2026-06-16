#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class AppController;
class QDockWidget;
class RibbonBarWidget;
class ViewLayoutWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    RibbonBarWidget *ribbonBar = nullptr;
    ViewLayoutWidget *viewLayout = nullptr;
    QDockWidget *projectPanelDock = nullptr;
    AppController *appController = nullptr;

    void setupWindow();
    void createMenuBar();
    void createCentralArea();
    void createRightPanel();
    void createStatusBar();
    void runDatabaseSmokeTest();
};
#endif // MAINWINDOW_H
