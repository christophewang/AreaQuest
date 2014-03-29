#ifndef CHARACTERRANKING_H
#define CHARACTERRANKING_H

#include <QStringList>
#include <QTcpSocket>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlRecord>
#include <QJsonValue>
#include <QJsonParseError>
#include "AreaStrings.h"
#include "Utils.h"

class CharacterRanking
{
private:
    QSqlDatabase areaquestDB;
public:
    CharacterRanking();
    void sendRankingTop(QTcpSocket *);
    void sendRankingMe(QTcpSocket *, const QString &);
    void getRanking(QTcpSocket *, const QString &, const QString &);
};

#endif // CHARACTERRANKING_H
