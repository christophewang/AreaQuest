#include "CharacterRanking.h"

CharacterRanking::CharacterRanking()
{
    areaquestDB = QSqlDatabase::database(AreaString::dbNameProram);
}

void CharacterRanking::sendRankingTop(QTcpSocket *socket)
{
    int i = 1;
    QSqlQuery query(areaquestDB);
    QJsonDocument rankingJson;
    QJsonObject record;
    QJsonArray recordsArray;

    if (query.exec(AreaString::getRankingTopQuery))
    {
        while (query.next())
        {
            record.insert(AreaString::positionJsonValue, i++);
            record.insert(AreaString::nicknameJsonValue, QJsonValue::fromVariant(query.value(AreaString::visibleNicknameJsonValue)));
            record.insert(AreaString::classJsonValue, QJsonValue::fromVariant(query.value(AreaString::classJsonValue)));
            record.insert(AreaString::levelJsonValue, QJsonValue::fromVariant(query.value(AreaString::lvlJsonValue)));
            recordsArray.push_back(record);
        }
        rankingJson.setArray(recordsArray);
        socket->write(rankingJson.toJson(QJsonDocument::Compact).append(AreaString::endl));
    }
    else
        socket->write(Utils::convertQString(AreaString::getRankingTopFailureMessage));
}

void CharacterRanking::sendRankingMe(QTcpSocket *socket, const QString &nickname)
{
    int i = 1;
    QSqlQuery query(areaquestDB);
    QJsonDocument rankingJson;
    QJsonObject record;

    if (query.exec(AreaString::getRankingMeQuery))
    {
        while (query.next())
        {
            if (query.value(AreaString::visibleNicknameJsonValue) == nickname)
            {
                record.insert(AreaString::positionJsonValue, i);
                record.insert(AreaString::nicknameJsonValue, QJsonValue::fromVariant(query.value(AreaString::visibleNicknameJsonValue)));
                record.insert(AreaString::classJsonValue, QJsonValue::fromVariant(query.value(AreaString::classJsonValue)));
                record.insert(AreaString::levelJsonValue, QJsonValue::fromVariant(query.value(AreaString::lvlJsonValue)));
                rankingJson.setObject(record);
                socket->write(rankingJson.toJson(QJsonDocument::Compact).append(AreaString::endl));
            }
            i++;
        }
    }
    else
        socket->write(Utils::convertQString(AreaString::getRankingMeFailureMessage));
}

void CharacterRanking::getRanking(QTcpSocket *socket, const QString &nickname, const QString &data)
{
    if (!data.compare(AreaString::getRankingTopCommand))
    {
        sendRankingTop(socket);
    }
    else if (!data.compare(AreaString::getRankingMeCommand))
    {
        sendRankingMe(socket, nickname);
    }
}
