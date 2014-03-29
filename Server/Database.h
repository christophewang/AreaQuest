#ifndef DATABASE_H
#define DATABASE_H

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "AreaStrings.h"
#include "Utils.h"

class Database
{
private:
    QSqlDatabase *areaquestDB;
    QSettings *settings;
public:
    Database(QSettings *);
    ~Database();
    void init();
};

#endif // DATABASE_H
