#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QSettings>
#include <QDateTime>
#include <QStringList>
#include <QNetworkInterface>
#include "Database.h"
#include "AccountManager.h"
#include "AreaStrings.h"
#include "Utils.h"


class Network: public QObject
{
    Q_OBJECT
private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
    AccountManager *accounts;
    QFile *log;
    QSettings *settings;
public:
    Network(QSettings *);
    ~Network();
    void launch();
    void parseData(QTcpSocket *, const QString &);
private slots:
    void newClient();
    void readData();
    void disconnectedClient();
};

#endif // NETWORK_H
