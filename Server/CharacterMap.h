#ifndef CHARACTERMAP_H
#define CHARACTERMAP_H

#include <QStringList>
#include <QTcpSocket>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "AreaStrings.h"
#include "Utils.h"

class CharacterMap
{
private:
    QSqlDatabase areaquestDB;
public:
    CharacterMap();
    void updatePosition(QTcpSocket *, const QStringList &);
    void exploreAround(QTcpSocket *, const QStringList &);
};

#endif // CHARACTERMAP_H
