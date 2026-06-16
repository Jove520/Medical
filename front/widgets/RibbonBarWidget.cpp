#include "RibbonBarWidget.h"

#include <QAction>
#include <QEvent>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QStringList>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVariant>

RibbonBarWidget::RibbonBarWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(76);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    ribbonStack = new QStackedWidget(this);
    ribbonStack->setFixedHeight(76);
    ribbonStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    ribbonStack->addWidget(createPage({
        "New Project", "Open", "Import DICOM", "Save", "Save As", "Export Report"
    }));

    ribbonStack->addWidget(createPage({
        "Pan", "Zoom", "Window/Level", "Rotate", "Reset View", "Project Panel"
    }));

    ribbonStack->addWidget(createPage({
        "3D", "MPR", "Layout", "Show Axis", "Show Landmarks"
    }));

    ribbonStack->addWidget(createPage({
        "Landmark", "Femoral Head", "Knee Center", "Ankle Center", "Mechanical Axis", "HKA Angle"
    }));

    ribbonStack->addWidget(createPage({
        "Preferences", "Calibration", "System Log"
    }));

    ribbonStack->setStyleSheet(
        "QStackedWidget {"
        "  background: #f7f9fc;"
        "  border-bottom: 1px solid #cfd6df;"
        "}"
        "QToolButton {"
        "  border: none;"
        "  padding: 4px 10px;"
        "  color: #1f2d3d;"
        "}"
        "QToolButton:hover {"
        "  background: #e8f1ff;"
        "  border: 1px solid #7fb4ff;"
        "}"
    );

    layout->addWidget(ribbonStack);
}

void RibbonBarWidget::bindMenuBar(QMenuBar *menuBar)
{
    if (boundMenuBar) {
        boundMenuBar->removeEventFilter(this);
    }

    boundMenuBar = menuBar;
    if (!boundMenuBar) {
        return;
    }

    const QList<QAction*> menuActions = boundMenuBar->actions();
    if (menuActions.size() > 5) {
        menuActions[0]->setProperty("ribbonPage", 0);
        menuActions[2]->setProperty("ribbonPage", 1);
        menuActions[3]->setProperty("ribbonPage", 2);
        menuActions[4]->setProperty("ribbonPage", 3);
        menuActions[5]->setProperty("ribbonPage", 4);
    }

    boundMenuBar->installEventFilter(this);
}

void RibbonBarWidget::setProjectPanelVisible(bool visible)
{
    if (projectPanelButton) {
        projectPanelButton->setChecked(visible);
    }
}

bool RibbonBarWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == boundMenuBar && event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = static_cast<QMouseEvent*>(event);
        QAction *menuAction = boundMenuBar->actionAt(mouseEvent->pos());

        if (menuAction && ribbonStack) {
            const QVariant ribbonPage = menuAction->property("ribbonPage");
            if (ribbonPage.isValid()) {
                ribbonStack->setCurrentIndex(ribbonPage.toInt());
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

QWidget* RibbonBarWidget::createPage(const QStringList& actions)
{
    QWidget *page = new QWidget(this);
    auto *layout = new QHBoxLayout(page);
    layout->setContentsMargins(8, 4, 8, 4);
    layout->setSpacing(8);

    for (const QString& text : actions) {
        QToolButton *button = createButton(text);
        if (text == "Project Panel") {
            projectPanelButton = button;
            projectPanelButton->setCheckable(true);
            connect(projectPanelButton, &QToolButton::clicked,
                    this, &RibbonBarWidget::projectPanelToggled);
        }
        layout->addWidget(button);
    }

    layout->addStretch();
    return page;
}

QToolButton* RibbonBarWidget::createButton(const QString& text)
{
    auto *button = new QToolButton(this);
    button->setText(text);
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setMinimumSize(88, 58);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    return button;
}
