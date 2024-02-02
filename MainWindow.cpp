#include "MainWindow.h"

#include "Database.h"
#include "TreeView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mDatabase(new Database("system.db", parent))
{
    setCentralWidget(new TreeView);

    resize(640, 480);

    mDatabase->open();
}
