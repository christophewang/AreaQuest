#include "Database.h"

Database::Database(QSettings *cSettings)
    : settings(cSettings)
{
}

Database::~Database()
{
    delete areaquestDB;
    delete settings;
}

void Database::init()
{
    Utils::printConsole(AreaString::initDatabaseMessage);
    if (settings->value(AreaString::dbMySQLValue).toInt() == 1)
    {
        areaquestDB = new QSqlDatabase(QSqlDatabase::addDatabase(AreaString::dbMySQLDriver, AreaString::dbNameProram));
        Utils::printConsole(AreaString::dbMySQLMessage);
        areaquestDB->setDatabaseName(AreaString::dbName);
        areaquestDB->setHostName(settings->value(AreaString::dbAddressValue).toString());
        areaquestDB->setUserName(AreaString::dbMySQLAdminName);
        areaquestDB->setPassword(AreaString::dbMySQLAdminPassword);
        areaquestDB->setConnectOptions(AreaString::dbMySQLOption);
        if (areaquestDB->open())
            Utils::printConsole(AreaString::launchDatabaseSuccessMessage);
        else
        {
            Utils::printConsole(AreaString::launchDatabaseFailureMessage);
            exit(EXIT_FAILURE);
        }
    }
    else if (settings->value(AreaString::dbPSQLValue).toInt() == 1)
    {
        areaquestDB = new QSqlDatabase(QSqlDatabase::addDatabase(AreaString::dbPSQLDriver, AreaString::dbNameProram));
        Utils::printConsole(AreaString::dbPSQLMessage);
        areaquestDB->setDatabaseName(AreaString::dbName);
        areaquestDB->setHostName(settings->value(AreaString::dbAddressValue).toString());
        areaquestDB->setUserName(AreaString::dbPSQLAdminName);
        areaquestDB->setPassword(AreaString::dbPSQLAdminPassword);
        if (areaquestDB->open())
            Utils::printConsole(AreaString::launchDatabaseSuccessMessage);
        else
        {
            Utils::printConsole(AreaString::launchDatabaseFailureMessage);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        Utils::printConsole(AreaString::launchDatabaseFailureMessage);
        exit(EXIT_FAILURE);
    }
}
