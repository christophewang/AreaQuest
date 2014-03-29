#include <QCoreApplication>
#include <QSettings>
#include "Network.h"
#include "Database.h"
#include "AreaStrings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(AreaString::organizationName);
    QCoreApplication::setOrganizationDomain(AreaString::organizationDomain);
    QCoreApplication::setApplicationName(AreaString::applicationName);
    QCoreApplication a(argc, argv);
    QSettings *settings = new QSettings(AreaString::settingsFile, QSettings::IniFormat);
    Database *areaquestDB;
    Network *areaquestServer;

    areaquestDB = new Database(settings);
    areaquestDB->init();
    areaquestServer = new Network(settings);
    areaquestServer->launch();
    return a.exec();
}
