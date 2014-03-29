#include "CharacterMessage.h"

CharacterMessage::CharacterMessage()
{
}

void CharacterMessage::sendMessage(QHash<QTcpSocket *, QString> players, QTcpSocket *socketA, QString data)
{
    QTcpSocket *socketB;
    QJsonParseError parse;
    QJsonDocument sendMessage;
    QJsonDocument receiveMessage;
    QJsonObject sendObject;
    QJsonObject receiveObject;

    data.remove(AreaString::sendMessageCommand);
    sendMessage = QJsonDocument::fromJson(data.toUtf8(), &parse);
    if (parse.error == QJsonParseError::NoError)
    {
        sendObject = sendMessage.object();
        socketB = players.key(sendObject.value(AreaString::nicknameJsonValue).toString(), 0);
        if (socketB)
        {
            receiveObject.insert(AreaString::nicknameJsonValue, players.value(socketA));
            receiveObject.insert(AreaString::messageJsonValue, sendObject.value(AreaString::messageJsonValue));
            receiveMessage.setObject(receiveObject);
            socketB->write(receiveMessage.toJson(QJsonDocument::Compact).insert(0, AreaString::receiveMessageCommand).append(AreaString::endl));
            socketA->write(Utils::convertQString(AreaString::sendSuccessMessage));
            Utils::printConsole(AreaString::sendSuccessInfoMessage.arg(players.value(socketA), sendObject.value(AreaString::nicknameJsonValue).toString()));
        }
        else
            socketA->write(Utils::convertQString(AreaString::sendNotConnectedFailureMessage));
    }
    else
        socketA->write(Utils::convertQString(AreaString::sendNotJsonFailureMessage));
}
