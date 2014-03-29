#include "CharacterInfo.h"

CharacterInfo::CharacterInfo()
{
    areaquestDB = QSqlDatabase::database(AreaString::dbNameProram);
}

bool CharacterInfo::checkExistingNickname(const QString &nickname)
{
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::getNicknameQuery))
    {
        while (query.next())
        {
            if (!nickname.compare(query.value(0).toString(), Qt::CaseInsensitive))
                return (true);
        }
    }
    return (false);
}

void CharacterInfo::getCharacterInfo(QTcpSocket *socket, const QStringList &data)
{
    QSqlQuery query(areaquestDB);
    QJsonDocument charInfo;
    QJsonObject records;
    QJsonArray recordsArray;
    QJsonObject recordStat;
    QJsonObject recordEquipment;
    QJsonObject recordInventory;

    switch (data.size())
    {
    case 2:
        if (!checkExistingNickname(data.at(1)))
        {
            socket->write(Utils::convertQString(AreaString::getCharacterInfoFailureMessage));
            break;
        }
        query.prepare(AreaString::getCharacterInfoQuery);
        query.bindValue(AreaString::nicknameBindValue, data.at(1));
        if (!query.exec())
        {
            socket->write(Utils::convertQString(AreaString::getCharacterInfoFailureMessage));
            break;
        }
        query.next();
        records.insert(AreaString::classJsonValue, QJsonValue::fromVariant(query.value(AreaString::classJsonValue)));
        records.insert(AreaString::levelJsonValue, QJsonValue::fromVariant(query.value(AreaString::lvlJsonValue)));
        records.insert(AreaString::experienceJsonValue, QJsonValue::fromVariant(query.value(AreaString::experienceJsonValue)));
        records.insert(AreaString::nextLevelJsonValue, QJsonValue::fromVariant(query.value(AreaString::nextLvlJsonValue)));
        records.insert(AreaString::moneyJsonValue, QJsonValue::fromVariant(query.value(AreaString::moneyJsonValue)));
        recordStat.insert(AreaString::hpJsonValue, QJsonValue::fromVariant(query.value(AreaString::hpJsonValue)));
        recordStat.insert(AreaString::hpMaxJsonValue, QJsonValue::fromVariant(query.value(AreaString::hpMaxJsonValue)));
        recordStat.insert(AreaString::mpJsonValue, QJsonValue::fromVariant(query.value(AreaString::mpJsonValue)));
        recordStat.insert(AreaString::mpMaxJsonValue, QJsonValue::fromVariant(query.value(AreaString::mpMaxJsonValue)));
        recordStat.insert(AreaString::attackJsonValue, QJsonValue::fromVariant(query.value(AreaString::attackJsonValue)));
        recordStat.insert(AreaString::defenseJsonValue, QJsonValue::fromVariant(query.value(AreaString::defenseJsonValue)));
        recordStat.insert(AreaString::abilityPowerJsonValue, QJsonValue::fromVariant(query.value(AreaString::abilityPowerJsonValue)));
        recordStat.insert(AreaString::magicResistJsonValue, QJsonValue::fromVariant(query.value(AreaString::magicResistJsonValue)));
        recordStat.insert(AreaString::agilityJsonValue, QJsonValue::fromVariant(query.value(AreaString::agilityJsonValue)));
        recordEquipment.insert(AreaString::idHeadgearJsonValue, QJsonValue::fromVariant(query.value(AreaString::headgearIdItemJsonValue)));
        recordEquipment.insert(AreaString::idArmorJsonValue, QJsonValue::fromVariant(query.value(AreaString::armorIdItemJsonValue)));
        recordEquipment.insert(AreaString::idLegsJsonValue, QJsonValue::fromVariant(query.value(AreaString::legsIdItemJsonValue)));
        recordEquipment.insert(AreaString::idShoesJsonValue, QJsonValue::fromVariant(query.value(AreaString::shoesIdItemJsonValue)));
        recordEquipment.insert(AreaString::idRighthandJsonValue, QJsonValue::fromVariant(query.value(AreaString::righthandIdItemJsonValue)));
        recordEquipment.insert(AreaString::idLefthandJsonValue, QJsonValue::fromVariant(query.value(AreaString::lefthandIdItemJsonValue)));
        records.insert(AreaString::statsJsonValue, recordStat);
        records.insert(AreaString::equipmentJsonValue, recordEquipment);
        query.prepare(AreaString::getItemListQuery);
        query.bindValue(AreaString::nicknameBindValue, data.at(1));
        if (!query.exec())
        {
            socket->write(Utils::convertQString(AreaString::getCharacterInfoFailureMessage));
            break;
        }
        while (query.next())
        {
            for(int x = 0; x < query.record().count(); x++)
                recordInventory.insert(query.record().fieldName(x), QJsonValue::fromVariant(query.value(x)));
            recordsArray.push_back(recordInventory);
        }
        records.insert(AreaString::inventoryJsonValue, recordsArray);
        charInfo.setObject(records);
        socket->write(charInfo.toJson(QJsonDocument::Compact).append(AreaString::endl));
        Utils::printConsole(AreaString::getCharacterInfoSuccessMessage.arg(data.at(1)));
        break;
    default:
        socket->write(Utils::convertQString(AreaString::getCharacterInfoParamFailureMessage));
        break;
    }
}

void CharacterInfo::updateCharacterInfo(QTcpSocket *socket, const QStringList &data)
{
    QSqlQuery query(areaquestDB);
    QJsonDocument charInfo;
    QJsonObject object;
    QJsonArray inventory;
    QJsonParseError parse;
    int idCharacter = 0;
    int idBank = 0;
    int idInventory = 0;
    int idEquipment = 0;
    int idStats = 0;

    switch (data.size())
    {
    case 3:
        charInfo = QJsonDocument::fromJson(data.at(2).toUtf8(), &parse);
        if (parse.error == QJsonParseError::NoError)
        {
            object = charInfo.object();
            inventory = object.value(AreaString::inventoryJsonValue).toArray();
            query.prepare(AreaString::getIdCharacterInfoQuery);
            query.bindValue(AreaString::nicknameBindValue, data.at(1));
            if (!query.exec())
            {
                socket->write(Utils::convertQString(AreaString::updateCharacterFailureMessage));
                break;
            }
            while (query.next())
            {
                idCharacter = query.value(AreaString::idCharacterJsonValue).toInt();
                idBank = query.value(AreaString::idBankJsonValue).toInt();
                idInventory = query.value(AreaString::idInventoryJsonValue).toInt();
                idEquipment = query.value(AreaString::idEquipmentJsonValue).toInt();
                idStats = query.value(AreaString::idCharacterStatsJsonValue).toInt();
            }
            query.prepare(AreaString::updateCharacterInfoQuery);
            query.bindValue(AreaString::idCharacterBindValue, idCharacter);
            query.bindValue(AreaString::idBankBindValue, idBank);
            query.bindValue(AreaString::idEquipmentBindValue, idEquipment);
            query.bindValue(AreaString::idCharacterStatsBindValue, idStats);
            query.bindValue(AreaString::idClassBindValue, object.value(AreaString::classJsonValue));
            query.bindValue(AreaString::idExperienceBindValue, object.value(AreaString::experienceJsonValue).toVariant());
            query.bindValue(AreaString::idLevelBindValue, object.value(AreaString::levelJsonValue).toVariant());
            query.bindValue(AreaString::idMoneyBindValue, object.value(AreaString::moneyJsonValue).toVariant());
            query.bindValue(AreaString::idNextLevelBindValue, object.value(AreaString::nextLevelJsonValue).toVariant());
            query.bindValue(AreaString::idHeadgearBindValue, object.value(AreaString::equipmentJsonValue).toObject().value(AreaString::idHeadgearJsonValue).toVariant());
            query.bindValue(AreaString::idArmorBindValue, object.value(AreaString::equipmentJsonValue).toObject().value(AreaString::idArmorJsonValue).toVariant());
            query.bindValue(AreaString::idLegsBindValue, object.value(AreaString::equipmentJsonValue).toObject().value(AreaString::idLegsJsonValue).toVariant());
            query.bindValue(AreaString::idShoesBindValue, object.value(AreaString::equipmentJsonValue).toObject().value(AreaString::idShoesJsonValue).toVariant());
            query.bindValue(AreaString::idRighthandBindValue, object.value(AreaString::equipmentJsonValue).toObject().value(AreaString::idRighthandJsonValue).toVariant());
            query.bindValue(AreaString::idLefthandBindValue, object.value(AreaString::equipmentJsonValue).toObject().value(AreaString::idLefthandJsonValue).toVariant());
            query.bindValue(AreaString::hpBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::hpJsonValue).toVariant());
            query.bindValue(AreaString::mpBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::mpJsonValue).toVariant());
            query.bindValue(AreaString::hpMaxBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::hpMaxJsonValue).toVariant());
            query.bindValue(AreaString::mpMaxBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::mpMaxJsonValue).toVariant());
            query.bindValue(AreaString::attackBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::attackJsonValue).toVariant());
            query.bindValue(AreaString::defenseBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::defenseJsonValue).toVariant());
            query.bindValue(AreaString::abilityPowerBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::abilityPowerJsonValue).toVariant());
            query.bindValue(AreaString::magicResistBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::magicResistJsonValue).toVariant());
            query.bindValue(AreaString::agilityBindValue, object.value(AreaString::statsJsonValue).toObject().value(AreaString::agilityJsonValue).toVariant());
            if (!query.exec())
            {
                socket->write(Utils::convertQString(AreaString::updateCharacterFailureMessage));
                break;
            }
            query.prepare(AreaString::deleteItemListQuery);
            query.bindValue(AreaString::idInventoryBindValue, idInventory);
            if (!query.exec())
            {
                socket->write(Utils::convertQString(AreaString::updateCharacterFailureMessage));
                break;
            }
            for (QJsonArray::const_iterator i = inventory.begin(); i != inventory.end(); i++)
            {
                query.prepare(AreaString::addItemListQuery);
                query.bindValue(AreaString::idInventoryBindValue, idInventory);
                query.bindValue(AreaString::idItemBindValue, (*i).toObject().value(AreaString::idItemJsonValue).toDouble());
                query.bindValue(AreaString::quantityBindValue, (*i).toObject().value(AreaString::quantityJsonValue).toDouble());
                if (!query.exec())
                {
                    socket->write(Utils::convertQString(AreaString::updateCharacterFailureMessage));
                    break;
                }
            }
            socket->write(Utils::convertQString(AreaString::updateCharacterSuccessMessage));
            Utils::printConsole(AreaString::updateCharacterInfoSuccessMessage.arg(data.at(1)));
        }
        else
            socket->write(Utils::convertQString(AreaString::updateCharacterFailureMessage));
        break;
    default:
        socket->write(Utils::convertQString(AreaString::updateCharacterParamFailureMessage));
        break;
    }
}
