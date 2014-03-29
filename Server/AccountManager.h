#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QHash>
#include <QStringList>
#include <QMap>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QPair>
#include <QList>
#include "CharacterInfo.h"
#include "CharacterMap.h"
#include "CharacterBattle.h"
#include "CharacterMessage.h"
#include "CharacterRanking.h"
#include "AreaStrings.h"
#include "Utils.h"


class AccountManager: public QObject
{
    Q_OBJECT
private:
    typedef void (CharacterInfo::*CharInfoPtr)(QTcpSocket *, const QStringList &);
    typedef void (CharacterMap::*CharMapPtr)(QTcpSocket *, const QStringList &);
    typedef void (CharacterBattle::*CharBattlePtr)(QHash<QTcpSocket *, QString>, QTcpSocket *, const QStringList &);
    typedef void (CharacterMessage::*CharMessagePtr)(QHash<QTcpSocket *, QString>, QTcpSocket *, QString);
    typedef void (CharacterRanking::*CharRankingPtr)(QTcpSocket *, const QString &, const QString &);
    QHash<QTcpSocket *, QString> players;
    CharacterInfo *charInfo;
    CharacterMap *charMap;
    CharacterBattle *charBattle;
    CharacterMessage *charMessage;
    CharacterRanking *charRanking;
    QMap<QString, CharInfoPtr> charInfoRequest;
    QMap<QString, CharMapPtr> charMapRequest;
    QMap<QString, CharBattlePtr> charBattleRequest;
    QMap<QString, CharMessagePtr> charMessageRequest;
    QMap<QString, CharRankingPtr> charRankingRequest;
    QStringList charInfoCommands;
    QStringList charMapCommands;
    QStringList charBattleCommands;
    QStringList charMessageCommands;
    QStringList charRankingCommands;
    QFile *log;
    QSqlDatabase areaquestDB;
public:
    AccountManager(QFile *);
    ~AccountManager();
    int checkExistingAccount(const QString &, const QString &);
    int createAccountHolder(const QString &, const QString &, const QString &);
    int createBankHolder(const QString &);
    int createInventoryHolder(const QString &);
    int createEquipmentHolder(const QString &);
    int createPositionHolder(const QString &);
    int createStatsHolder(const QString &);
    void createCharacterHolder(QTcpSocket *, const QString &, const QString &, const QString &);
    void createAccount(QTcpSocket *, const QString &, const QString &, const QString &);
    int checkAuthAccount(const QString &, const QString &, QString *);
    void setOnlineStatus(const QString &);
    void setOfflineStatus(const QString &);
    void logAccount(QTcpSocket *, const QString &, const QString &);
    void parseData(QTcpSocket *, const QString &);
private slots:
    void disconnectedPlayer();
    void readData();
};

#endif // ACCOUNTMANAGER_H
