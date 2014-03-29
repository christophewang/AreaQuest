#ifndef CHARACTERBATTLE_H
#define CHARACTERBATTLE_H

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
#include <QDebug>
#include <QPair>
#include <QList>
#include <QtGlobal>
#include "AreaStrings.h"
#include "Utils.h"

class CharacterBattle: public QObject
{
    Q_OBJECT
private:
    QSqlDatabase areaquestDB;
    QList<QPair<QString, QString> > battles;

public:
    CharacterBattle();
    void checkDisconnection(QHash<QTcpSocket *, QString>, QTcpSocket *);
    QJsonObject getPlayerStats(const QString &);
    QString getOpponentName(QHash<QTcpSocket *, QString>, QTcpSocket *, bool);
    int checkVictory(QHash<QTcpSocket *, QString>, QTcpSocket *, QTcpSocket *, QJsonDocument);
    void abandonBattle(QHash<QTcpSocket *, QString>, QTcpSocket *);
    void battleTurnFinished(QHash<QTcpSocket *, QString>, QTcpSocket *, const QStringList &);
    void battle(QHash<QTcpSocket *, QString>, QTcpSocket *, const QStringList &);
    void sendBattleRequest(QHash<QTcpSocket *, QString>, QTcpSocket *, QTcpSocket *, const QStringList &);
    void acceptBattleRequest(QHash<QTcpSocket *, QString>, QTcpSocket *, QTcpSocket *, const QStringList &);
    void battleRequest(QHash<QTcpSocket *, QString>, QTcpSocket *, const QStringList &);
};

#endif // CHARACTERBATTLE_H
