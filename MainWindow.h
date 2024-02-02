#pragma once

#include <QMainWindow>

class Database;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Database *mDatabase = nullptr;
};
