#pragma once

#include <QObject>

#include <string>

struct sqlite3;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(std::string_view filepath, QObject *parent = nullptr);
    ~Database();

    bool open();

private:
    sqlite3 *mDB = nullptr;
    std::string mFilepath;
};
