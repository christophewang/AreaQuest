#include "Network.h"

Network::Network(QSettings *cSettings)
    : settings(cSettings)
{
    Utils::printConsole(AreaString::initMessage);
    server = new QTcpServer;
    log = new QFile(AreaString::logFile);
    accounts = new AccountManager(log);
}

Network::~Network()
{
    log->close();
    delete log;
    delete accounts;
    delete server;
    delete settings;
}

void Network::launch()
{
    if (server->listen(QHostAddress::Any, settings->value(AreaString::portValue).toInt()))
    {
        Utils::printConsole(AreaString::launchSuccessMessage.arg(QDateTime::currentDateTime().toString(AreaString::dateFormat)));
        log->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));
    }
    else
    {
        Utils::printConsole(AreaString::launchFailureMessage);
        exit(EXIT_FAILURE);
    }
}

void Network::newClient()
{
    QTcpSocket *newClient = server->nextPendingConnection();

    newClient->write(Utils::convertQString(AreaString::connectedMessage));
    Utils::printConsole(AreaString::clientInfoMessage.arg(QDateTime::currentDateTime().toString(AreaString::dateTimeFormat), newClient->peerAddress().toString()));
    connect(newClient, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(newClient, SIGNAL(disconnected()), this, SLOT(disconnectedClient()));
    clients << newClient;
}

void Network::disconnectedClient()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    Utils::printConsole(AreaString::disconnectedMessage.arg(QDateTime::currentDateTime().toString(AreaString::dateTimeFormat), socket->peerAddress().toString()));
    clients.removeOne(socket);
    socket->deleteLater();
}

void Network::readData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QString data;
    QTextStream in(socket);

    data = in.readLine();
    Utils::writeLog(log, AreaString::clientCommand.arg(QDateTime::currentDateTime().toString(AreaString::dateTimeFormat), socket->peerAddress().toString(), data).append(AreaString::endl));
    parseData(socket, data);
}

void Network::parseData(QTcpSocket *socket, const QString &data)
{
    QStringList input = data.split(AreaString::space);

    if (!input.at(0).compare(AreaString::createAccountCommand) && input.size() == 4)
        accounts->createAccount(socket, input.at(1), input.at(2), input.at(3));
    else if (!input.at(0).compare(AreaString::loginAccountCommand) && input.size() == 3)
        accounts->logAccount(socket, input.at(1), input.at(2));
    else
        socket->write(Utils::convertQString(AreaString::invalidCommand));
}
