#include "MainWindow.h"

#include <QSettings>
#include <QDebug>

#include "TreeView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new TreeView);

    resize(640, 480);

    QSettings settings("Ozzylogik", "Countries");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("Ozzylogik", "Countries");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);
}
