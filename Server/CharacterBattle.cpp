#include "CharacterBattle.h"

CharacterBattle::CharacterBattle()
{
    areaquestDB = QSqlDatabase::database(AreaString::dbNameProram);
}

void CharacterBattle::checkDisconnection(QHash<QTcpSocket *, QString> players, QTcpSocket *socket)
{
    for (int i = 0; i < battles.size(); ++i)
    {
        if (battles.at(i).first == players.value(socket))
        {
            Utils::printConsole(AreaString::battleInterruptedInfoMessage.arg(battles.at(i).second, battles.at(i).first));
            players.key(battles.at(i).second)->write(Utils::convertQString(AreaString::battleDecisionWinMessage));
            battles.removeAt(i);
        }
        else if (battles.at(i).second == players.value(socket))
        {
            Utils::printConsole(AreaString::battleInterruptedInfoMessage.arg(battles.at(i).first, battles.at(i).second));
            players.key(battles.at(i).first)->write(Utils::convertQString(AreaString::battleDecisionWinMessage));
            battles.removeAt(i);
        }
    }
}

QJsonObject CharacterBattle::getPlayerStats(const QString &nickname)
{
    QSqlQuery query(areaquestDB);
    QJsonObject record;

    query.prepare(AreaString::getPlayerStatsQuery);
    query.bindValue(AreaString::nicknameBindValue, nickname);
    if (query.exec())
    {
        while (query.next())
        {
            record.insert(AreaString::nicknameJsonValue, nickname);
            record.insert(AreaString::classJsonValue, QJsonValue::fromVariant(query.value(AreaString::classJsonValue)));
            record.insert(AreaString::levelJsonValue, QJsonValue::fromVariant(query.value(AreaString::lvlJsonValue)));
            record.insert(AreaString::hpJsonValue, QJsonValue::fromVariant(query.value(AreaString::hpJsonValue)));
            record.insert(AreaString::hpMaxJsonValue, QJsonValue::fromVariant(query.value(AreaString::hpMaxJsonValue)));
            record.insert(AreaString::mpJsonValue, QJsonValue::fromVariant(query.value(AreaString::mpJsonValue)));
            record.insert(AreaString::mpMaxJsonValue, QJsonValue::fromVariant(query.value(AreaString::mpMaxJsonValue)));
            record.insert(AreaString::attackJsonValue, QJsonValue::fromVariant(query.value(AreaString::attackJsonValue)));
            record.insert(AreaString::defenseJsonValue, QJsonValue::fromVariant(query.value(AreaString::defenseJsonValue)));
            record.insert(AreaString::abilityPowerJsonValue, QJsonValue::fromVariant(query.value(AreaString::abilityPowerJsonValue)));
            record.insert(AreaString::magicResistJsonValue, QJsonValue::fromVariant(query.value(AreaString::magicResistJsonValue)));
            record.insert(AreaString::agilityJsonValue, QJsonValue::fromVariant(query.value(AreaString::agilityJsonValue)));
            record.insert(AreaString::attackNameJsonValue, QJsonValue::fromVariant(AreaString::blank));
            record.insert(AreaString::damageJsonValue, QJsonValue::fromVariant(0));
        }
    }
    return record;
}

QString CharacterBattle::getOpponentName(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, bool battleEnded)
{
    QString temp;

    for (int i = 0; i < battles.size(); ++i)
    {
        if (battles.at(i).first == players.value(socketA))
        {
            temp = battles.at(i).second;
            if (battleEnded)
                battles.removeAt(i);
        }
        else if (battles.at(i).second == players.value(socketA))
        {
            temp = battles.at(i).first;
            if (battleEnded)
                battles.removeAt(i);
        }
    }
    return temp;
}

int CharacterBattle::checkVictory(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, QTcpSocket *socketB, QJsonDocument charInfo)
{
    double playerAHp = 0;
    double playerBHp = 0;
    QJsonObject object;
    QJsonArray playersArray;

    object = charInfo.object();
    playersArray = object.value(AreaString::playersJsonValue).toArray();
    for (QJsonArray::const_iterator i = playersArray.begin(); i != playersArray.end(); i++)
    {
        if (!players.value(socketA).compare((*i).toObject().value(AreaString::nicknameJsonValue).toString()))
            playerAHp = (*i).toObject().value(AreaString::hpJsonValue).toDouble();
        else if (!players.value(socketB).compare((*i).toObject().value(AreaString::nicknameJsonValue).toString()))
            playerBHp = (*i).toObject().value(AreaString::hpJsonValue).toDouble();
    }
    if (playerAHp <= 0)
    {
        socketA->write(Utils::convertQString(AreaString::battleDecisionLossMessage));
        socketB->write(Utils::convertQString(AreaString::battleDecisionWinMessage));
        Utils::printConsole(AreaString::battleResultInfoMessage.arg(players.value(socketB), players.value(socketA)));
        return 1;
    }
    else if (playerBHp <= 0)
    {
        socketA->write(Utils::convertQString(AreaString::battleDecisionWinMessage));
        socketB->write(Utils::convertQString(AreaString::battleDecisionLossMessage));
        Utils::printConsole(AreaString::battleResultInfoMessage.arg(players.value(socketA), players.value(socketB)));
        return 1;
    }
    return 0;
}

void CharacterBattle::abandonBattle(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA)
{
    QTcpSocket *socketB;

    socketB = players.key(getOpponentName(players, socketA, true), 0);
    if (socketB)
    {
        socketA->write(Utils::convertQString(AreaString::battleDecisionLossMessage));
        socketB->write(Utils::convertQString(AreaString::battleDecisionWinMessage));
        Utils::printConsole(AreaString::battleResultInfoMessage.arg(players.value(socketB), players.value(socketA)));
    }
}

void CharacterBattle::battleTurnFinished(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, const QStringList &data)
{
    QJsonDocument charInfo;
    QJsonParseError parse;
    QTcpSocket *socketB;

    charInfo = QJsonDocument::fromJson(data.at(1).toUtf8(), &parse);
    if (parse.error == QJsonParseError::NoError && (socketB = players.key(getOpponentName(players, socketA, false), 0)))
    {
        if (!checkVictory(players, socketA, socketB, charInfo))
            socketB->write(charInfo.toJson(QJsonDocument::Compact).insert(0, AreaString::space).insert(0, AreaString::battleTurnStartCommand).append(AreaString::endl));
        else
            getOpponentName(players, socketA, true);
    }
    else
        socketA->write(Utils::convertQString(AreaString::battleFailureMessage));
}

void CharacterBattle::battle(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, const QStringList &data)
{
    switch (data.size())
    {
    case 1:
        if (!data.at(0).compare(AreaString::battleGiveUpCommand))
            abandonBattle(players, socketA);
        break;
    case 2:
        if (!data.at(0).compare(AreaString::battleTurnFinishCommand))
            battleTurnFinished(players, socketA, data);
        break;
    default:
        socketA->write(Utils::convertQString(AreaString::battleFailureMessage));
        break;
    }
}

void CharacterBattle::sendBattleRequest(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, QTcpSocket *socketB, const QStringList &data)
{
    QSqlQuery query(areaquestDB);
    QJsonObject record;
    QJsonDocument charInfo;

    query.prepare(AreaString::getPlayerInfoQuery);
    query.bindValue(AreaString::nicknameBindValue, players.value(socketA));
    if (query.exec())
    {
        while (query.next())
        {
            record.insert(AreaString::nicknameJsonValue, players.value(socketA));
            record.insert(AreaString::classJsonValue, QJsonValue::fromVariant(query.value(AreaString::classJsonValue)));
            record.insert(AreaString::levelJsonValue, QJsonValue::fromVariant(query.value(AreaString::lvlJsonValue)));
            charInfo.setObject(record);
        }
        socketB->write(charInfo.toJson(QJsonDocument::Compact).insert(0, AreaString::space).insert(0, AreaString::battleRequestCommand).append(AreaString::endl));
        Utils::printConsole(AreaString::battleRequestInfoMessage.arg(players.value(socketA), data.at(1)));
    }
}

void CharacterBattle::acceptBattleRequest(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, QTcpSocket *socketB, const QStringList &data)
{
    QPair<QString, QString> battlePlayers;
    QJsonDocument charInfo;
    QJsonObject playersStats;
    QJsonArray playerStats;

    Utils::printConsole(AreaString::battleStartInfoMessage.arg(players.value(socketA), data.at(1)));
    battlePlayers.first = players.value(socketA);
    battlePlayers.second = players.value(socketB);
    battles.push_front(battlePlayers);
    playerStats.push_back(getPlayerStats(players.value(socketA)));
    playerStats.push_back(getPlayerStats(players.value(socketB)));
    playersStats.insert(AreaString::playersJsonValue, playerStats);
    charInfo.setObject(playersStats);
    qsrand(qrand());
    if (qrand() % 2)
    {
        socketA->write(charInfo.toJson(QJsonDocument::Compact).insert(0, AreaString::space).insert(0, AreaString::battleTurnStartCommand).append(AreaString::endl));
        socketB->write(charInfo.toJson(QJsonDocument::Compact).insert(0, AreaString::battleTurnWaitCommand).append(AreaString::endl));
    }
    else
    {
        socketB->write(charInfo.toJson(QJsonDocument::Compact).insert(0, AreaString::space).insert(0, AreaString::battleTurnStartCommand).append(AreaString::endl));
        socketA->write(charInfo.toJson(QJsonDocument::Compact).insert(0, AreaString::battleTurnWaitCommand).append(AreaString::endl));
    }
}

void CharacterBattle::battleRequest(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, const QStringList &data)
{
    QTcpSocket *socketB;

    switch (data.size())
    {
    case 2:
    {
        socketB = players.key(data.at(1), 0);
        if (!data.at(0).compare(AreaString::battleRequestCommand))
        {
            if (socketB && (socketB != socketA))
                sendBattleRequest(players, socketA, socketB, data);
            else
                socketA->write(Utils::convertQString(AreaString::battleRequestFailureMessage));
        }
        else if (!data.at(0).compare(AreaString::battleRequestAcceptCommand))
        {
            if (socketB &&(socketB != socketA))
                acceptBattleRequest(players, socketA, socketB, data);
            else
                socketA->write(Utils::convertQString(AreaString::battleRequestAcceptFailureMessage));
        }
        else if (!data.at(0).compare(AreaString::battleRequestDenyCommand))
        {
            if (socketB &&(socketB != socketA))
            {
                socketB->write(Utils::convertQString(AreaString::battleRequestDeniedMessage));
                Utils::printConsole(AreaString::battleDeniedInfoMessage.arg(players.value(socketA), data.at(1)));
            }
            else
                socketA->write(Utils::convertQString(AreaString::battleRequestDenyFailureMessage));
        }
        break;
    }
    default:
        socketA->write(Utils::convertQString(AreaString::battleRequestFailureMessage));
        break;
    }
}
