#include "MainWindow.h"

#include "TreeView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new TreeView("system.db"));

    resize(640, 480);
}
