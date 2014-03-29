#include "CharacterMap.h"

CharacterMap::CharacterMap()
{
    areaquestDB = QSqlDatabase::database(AreaString::dbNameProram);
}

void CharacterMap::updatePosition(QTcpSocket *socket, const QStringList &data)
{
    QSqlQuery query(areaquestDB);

    switch (data.size())
    {
    case 4:
        query.prepare(AreaString::updatePositionQuery);
        query.bindValue(AreaString::nicknameBindValue, data.at(1));
        query.bindValue(AreaString::latitudeBindValue, data.at(2));
        query.bindValue(AreaString::longitudeBindValue, data.at(3));
        if (query.exec())
        {
            socket->write(Utils::convertQString(AreaString::updatePositionSuccessMessage));
            Utils::printConsole(AreaString::updatePositionInfoSuccessMessage.arg(data.at(1)));
        }
        else
            socket->write(Utils::convertQString(AreaString::updatePositionFailureMessage));
        break;
    default:
        socket->write(Utils::convertQString(AreaString::updatePositionParamFailureMessage));
        break;
    }
}

void CharacterMap::exploreAround(QTcpSocket *socket, const QStringList &data)
{
    QSqlQuery query(areaquestDB);
    QJsonDocument charInfo;
    QJsonArray recordsArray;
    QJsonObject record;

    switch (data.size())
    {
    case 2:
        query.prepare(AreaString::exploreAroundQuery);
        query.bindValue(AreaString::nicknameBindValue, data.at(1));
        query.exec();
        while (query.next())
        {
            record.insert(AreaString::nicknameJsonValue, QJsonValue::fromVariant(query.value(AreaString::visibleNicknameJsonValue)));
            record.insert(AreaString::classJsonValue, QJsonValue::fromVariant(query.value(AreaString::classJsonValue)));
            record.insert(AreaString::levelJsonValue, QJsonValue::fromVariant(query.value(AreaString::lvlJsonValue)));
            record.insert(AreaString::latitudeJsonValue, QJsonValue::fromVariant(query.value(AreaString::latitudeJsonValue)));
            record.insert(AreaString::longitudeJsonValue, QJsonValue::fromVariant(query.value(AreaString::longitudeJsonValue)));
            record.insert(AreaString::distanceJsonValue, QJsonValue::fromVariant(query.value(AreaString::distanceJsonValue)));
            recordsArray.push_back(record);
        }
        charInfo.setArray(recordsArray);
        socket->write(charInfo.toJson(QJsonDocument::Compact).append(AreaString::endl));
        Utils::printConsole(AreaString::exploreAroundInfoSuccessMessage.arg(data.at(1)));
        break;
    default:
        socket->write(Utils::convertQString(AreaString::exploreAroundParamFailureMessage));
        break;
    }
}
