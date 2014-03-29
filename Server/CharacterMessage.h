#ifndef CHARACTERMESSAGE_H
#define CHARACTERMESSAGE_H

#include <QStringList>
#include <QTcpSocket>
#include <QObject>
#include <QHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextCodec>
#include <QDebug>
#include "AreaStrings.h"
#include "Utils.h"

class CharacterMessage
{
public:
    CharacterMessage();
    void sendMessage(QHash<QTcpSocket *, QString>, QTcpSocket *, QString);
};

#endif // CHARACTERMESSAGE_H
