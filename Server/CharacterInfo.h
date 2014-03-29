#ifndef CHARACTERINFO_H
#define CHARACTERINFO_H

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

class CharacterInfo
{
private:
    QSqlDatabase areaquestDB;
public:
    CharacterInfo();
    bool checkExistingNickname(const QString &);
    void getCharacterInfo(QTcpSocket *, const QStringList &);
    void updateCharacterInfo(QTcpSocket *, const QStringList &);

};

#endif // CHARACTERINFO_H
