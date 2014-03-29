#include "AccountManager.h"

AccountManager::AccountManager(QFile *cLog)
    : log(cLog)
{
    areaquestDB = QSqlDatabase::database(AreaString::dbNameProram);
    charInfo = new CharacterInfo();
    charMap = new CharacterMap();
    charBattle = new CharacterBattle();
    charMessage = new CharacterMessage();
    charRanking = new CharacterRanking();
    charInfoCommands << AreaString::getCharCommand
                     << AreaString::updateCharCommand;
    charMapCommands  << AreaString::updatePosCommand
                     << AreaString::exploreAroundCommand;
    charBattleCommands << AreaString::battleRequestCommand
                       << AreaString::battleRequestAcceptCommand
                       << AreaString::battleRequestDenyCommand
                       << AreaString::battleTurnFinishCommand
                       << AreaString::battleTurnStartCommand
                       << AreaString::battleGiveUpCommand;
    charMessageCommands << AreaString::sendMessageCommand;
    charRankingCommands << AreaString::getRankingTopCommand
                        << AreaString::getRankingMeCommand;
    charInfoRequest[AreaString::getCharCommand] = &CharacterInfo::getCharacterInfo;
    charInfoRequest[AreaString::updateCharCommand] = &CharacterInfo::updateCharacterInfo;
    charMapRequest[AreaString::updatePosCommand] = &CharacterMap::updatePosition;
    charMapRequest[AreaString::exploreAroundCommand] = &CharacterMap::exploreAround;
    charBattleRequest[AreaString::battleRequestCommand] = &CharacterBattle::battleRequest;
    charBattleRequest[AreaString::battleRequestAcceptCommand] = &CharacterBattle::battleRequest;
    charBattleRequest[AreaString::battleRequestDenyCommand] = &CharacterBattle::battleRequest;
    charBattleRequest[AreaString::battleTurnFinishCommand] = &CharacterBattle::battle;
    charBattleRequest[AreaString::battleTurnStartCommand] = &CharacterBattle::battle;
    charBattleRequest[AreaString::battleGiveUpCommand] = &CharacterBattle::battle;
    charMessageRequest[AreaString::sendMessageCommand] = &CharacterMessage::sendMessage;
    charRankingRequest[AreaString::getRankingTopCommand] = &CharacterRanking::getRanking;
    charRankingRequest[AreaString::getRankingMeCommand] = &CharacterRanking::getRanking;
}

AccountManager::~AccountManager()
{
    delete charInfo;
    delete charMap;
    delete charBattle;
    delete charMessage;
}

int AccountManager::checkExistingAccount(const QString &email, const QString &nickname)
{
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::getEmailNicknameQuery))
    {
        while (query.next())
        {
            if (!email.compare(query.value(0).toString(), Qt::CaseInsensitive))
                return (1);
            if (!nickname.compare(query.value(1).toString(), Qt::CaseInsensitive))
                return (2);
        }
    }
    else
        return (-1);
    return (0);
}

int AccountManager::createAccountHolder(const QString &email, const QString &password, const QString &nickname)
{
    int idAccount = 0;
    QSqlQuery query(areaquestDB);

    query.prepare(AreaString::registerAccountQuery);
    query.bindValue(AreaString::emailBindValue, email);
    query.bindValue(AreaString::passwordBindValue, password);
    query.bindValue(AreaString::nicknameBindValue, nickname);
    if (query.exec())
    {
        idAccount = query.lastInsertId().toInt();
        Utils::printConsole(AreaString::registerAccountInfoSuccessMessage.arg(email).arg(idAccount));
    }
    else
        Utils::printConsole(AreaString::registerAccountInfoFailureMessage.arg(email));
    return (idAccount);
}

int AccountManager::createBankHolder(const QString &email)
{
    int idBank = 0;
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::createBankQuery))
    {
        idBank = query.lastInsertId().toInt();
        Utils::printConsole(AreaString::createBankInfoSuccessMessage.arg(email).arg(idBank));
    }
    else
        Utils::printConsole(AreaString::createBankInfoFailureMessage.arg(email));
    return (idBank);
}

int AccountManager::createInventoryHolder(const QString &email)
{
    int idInventory = 0;
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::createInventoryQuery))
    {
        idInventory = query.lastInsertId().toInt();
        Utils::printConsole(AreaString::createInventoryInfoSuccessMessage.arg(email).arg(idInventory));
    }
    else
        Utils::printConsole(AreaString::createInventoryInfoFailureMessage.arg(email));
    return (idInventory);
}

int AccountManager::createEquipmentHolder(const QString &email)
{
    int idEquipment = 0;
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::createEquipmentQuery))
    {
        idEquipment = query.lastInsertId().toInt();
        Utils::printConsole(AreaString::createEquipmentInfoSuccessMessage.arg(email).arg(idEquipment));
    }
    else
        Utils::printConsole(AreaString::createEquipmentInfoFailureMessage.arg(email));
    return (idEquipment);
}

int AccountManager::createPositionHolder(const QString &email)
{
    int idPosition = 0;
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::createPositionQuery))
    {
        idPosition = query.lastInsertId().toInt();
        Utils::printConsole(AreaString::createPositionInfoSuccessMessage.arg(email).arg(idPosition));
    }
    else
        Utils::printConsole(AreaString::createPositionInfoFailureMessage.arg(email));
    return (idPosition);
}

int AccountManager::createStatsHolder(const QString &email)
{
    int idStats = 0;
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::createStatsQuery))
    {
        idStats = query.lastInsertId().toInt();
        Utils::printConsole(AreaString::createInventoryInfoSuccessMessage.arg(email).arg(idStats));
    }
    else
        Utils::printConsole(AreaString::createStatsInfoFailureMessage.arg(email));
    return (idStats);
}

void AccountManager::createCharacterHolder(QTcpSocket *socket, const QString &email, const QString &password, const QString &nickname)
{
    int idAccount;
    int idBank;
    int idInventory;
    int idEquipment;
    int idPosition;
    int idStats;
    QSqlQuery query(areaquestDB);

    idAccount = createAccountHolder(email, password, nickname);
    idBank = createBankHolder(email);
    idInventory = createInventoryHolder(email);
    idEquipment = createEquipmentHolder(email);
    idPosition = createPositionHolder(email);
    idStats = createStatsHolder(email);
    query.prepare(AreaString::createCharacterInfoQuery);
    query.bindValue(AreaString::idAccountBindValue, idAccount);
    query.bindValue(AreaString::idBankBindValue, idBank);
    query.bindValue(AreaString::idInventoryBindValue, idInventory);
    query.bindValue(AreaString::idEquipmentBindValue, idEquipment);
    query.bindValue(AreaString::idPositionBindValue, idPosition);
    query.bindValue(AreaString::idStatsBindValue, idStats);
    if (!query.exec() || !idAccount || !idBank || !idInventory || !idEquipment || !idPosition || !idStats)
    {
        socket->write(Utils::convertQString(AreaString::registerAccountFailureMessage));
        Utils::printConsole(AreaString::createCharacterInfoFailureMessage.arg(nickname, email));
    }
}

void AccountManager::createAccount(QTcpSocket *socket, const QString &email, const QString &password, const QString &nickname)
{
    Utils::printConsole(AreaString::registerAccountMessage);
    switch (checkExistingAccount(email, nickname))
    {
    case -1:
        socket->write(Utils::convertQString(AreaString::registerAccountFailureMessage));
        Utils::printConsole(AreaString::checkExistingFailureMessage);
        break;
    case 0:
        createCharacterHolder(socket, email, password, nickname);
        socket->write(Utils::convertQString(AreaString::registerAccountSuccessMessage));
        Utils::printConsole(AreaString::createCharacterInfoSuccessMessage.arg(nickname, email));
        break;
    case 1:
        socket->write(Utils::convertQString(AreaString::createEmailFailureMessage));
        Utils::printConsole(AreaString::createEmailInfoFailureMessage.arg(email));
        break;
    case 2:
        socket->write(Utils::convertQString(AreaString::createNicknameFailureMessage));
        Utils::printConsole(AreaString::createNicknameInfoFailureMessage.arg(email));
        break;
    }
}

int AccountManager::checkAuthAccount(const QString &email, const QString &password, QString *nickname)
{
    QSqlQuery query(areaquestDB);

    if (query.exec(AreaString::getEmailPasswordNicknameQuery))
    {
        while (query.next())
        {
            if (!email.compare(query.value(0).toString()) && password.compare(query.value(1).toString()))
                return (1);
            if (!email.compare(query.value(0).toString()) && !password.compare(query.value(1).toString()))
            {
                nickname->push_back(query.value(2).toString());
                return (2);
            }
        }
    }
    else
        return (-1);
    return (0);
}

void AccountManager::setOnlineStatus(const QString &nickname)
{
    QSqlQuery query(areaquestDB);

    query.prepare(AreaString::setOnlineStatusQuery);
    query.bindValue(AreaString::nicknameBindValue, nickname);
    if (query.exec())
        Utils::printConsole(AreaString::playerOnlineInfoMessage.arg(nickname));
    else
        Utils::printConsole(AreaString::playerOnlineInfoFailureMessage.arg(nickname));
}

void AccountManager::setOfflineStatus(const QString &nickname)
{
    QSqlQuery query(areaquestDB);

    query.prepare(AreaString::setOfflineStatusQuery);
    query.bindValue(AreaString::nicknameBindValue, nickname);
    if (query.exec())
        Utils::printConsole(AreaString::playerOfflineInfoMessage.arg(nickname));
    else
        Utils::printConsole(AreaString::playerOfflineInfoFailureMessage.arg(nickname));
}

void AccountManager::logAccount(QTcpSocket *socket, const QString &email, const QString &password)
{
    QString nickname;

    switch (checkAuthAccount(email, password, &nickname))
    {
    case -1:
        socket->write(Utils::convertQString(AreaString::loginAccountPasswordFailureMessage));
        Utils::printConsole(AreaString::checkAuthFailureMessage);
        break;
    case 0:
        socket->write(Utils::convertQString(AreaString::loginAccountEmailFailureMessage));
        Utils::printConsole(AreaString::loginAccountEmailInfoFailureMessage.arg(email));
        break;
    case 1:
        socket->write(Utils::convertQString(AreaString::loginAccountPasswordFailureMessage));
        Utils::printConsole(AreaString::loginAccountPasswordInfoFailureMessage.arg(email));
        break;
    case 2:
        socket->write(Utils::convertQString(AreaString::loginAccountSuccessMessage.arg(nickname)));
        Utils::printConsole(AreaString::loginAccountInfoSuccessMessage.arg(email));
        setOnlineStatus(nickname);
        players.insert(socket, nickname);
        disconnect(socket, SIGNAL(readyRead()), 0, 0);
        connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedPlayer()));
        break;
    }
}

void AccountManager::disconnectedPlayer()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    setOfflineStatus(players.value(socket));
    charBattle->checkDisconnection(players, socket);
    players.remove(socket);
}

void AccountManager::readData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QTextStream in(socket);
    QString data;

    data = in.readLine();
    Utils::writeLog(log, AreaString::clientCommand.arg(QDateTime::currentDateTime().toString(AreaString::dateTimeFormat), players.value(socket), data).append(AreaString::endl));
    Utils::printConsole(AreaString::clientCommand.arg(QDateTime::currentDateTime().toString(AreaString::dateTimeFormat), players.value(socket), data));
    parseData(socket, data);
}

void AccountManager::parseData(QTcpSocket *socket, const QString& data)
{
    QStringList input = data.split(AreaString::space);

    for (int i = 0; i < charInfoCommands.size(); ++i)
    {
        if (!charInfoCommands.at(i).compare(input.at(0)))
        {
            (charInfo->*charInfoRequest[input.at(0)])(socket, input);
            return;
        }
    }
    for (int i = 0; i < charMapCommands.size(); ++i)
    {
        if (!charMapCommands.at(i).compare(input.at(0)))
        {
            (charMap->*charMapRequest[input.at(0)])(socket, input);
            return;
        }
    }
    for (int i = 0; i < charBattleCommands.size(); ++i)
    {
        if (!charBattleCommands.at(i).compare(input.at(0)))
        {
            (charBattle->*charBattleRequest[input.at(0)])(players, socket, input);
            return;
        }
    }
    for (int i = 0; i < charMessageCommands.size(); ++i)
    {
        if (!charMessageCommands.at(i).compare(input.at(0)))
        {
            (charMessage->*charMessageRequest[input.at(0)])(players, socket, data);
            return;
        }
    }
    for (int i = 0; i < charRankingCommands.size(); ++i)
    {
        if (!charRankingCommands.at(i).compare(input.at(0)))
        {
            (charRanking->*charRankingRequest[input.at(0)])(socket, players.value(socket), data);
            return;
        }
    }
    socket->write(Utils::convertQString(AreaString::invalidCommand));
}
