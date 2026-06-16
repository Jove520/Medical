#include "LogPanelWidget.h"

#include <QHeaderView>

LogPanelWidget::LogPanelWidget(QWidget *parent)
    : QTableWidget(3, 3, parent)
{
    setHorizontalHeaderLabels({"Time", "Level", "Message"});
    verticalHeader()->setVisible(false);
    horizontalHeader()->setStretchLastSection(true);

    setItem(0, 0, new QTableWidgetItem("Ready"));
    setItem(0, 1, new QTableWidgetItem("INFO"));
    setItem(0, 2, new QTableWidgetItem("Application initialized."));
}
