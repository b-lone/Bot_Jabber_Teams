#include "botserver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "botconfig.h"
#include "botcommon.h"
#include "botnetworkmanager.h"
#include "botmessage.h"
#include "botstore.h"
#include "botconfig.h"

BotServer *BotServer::Instance()
{
    static BotServer * s = new BotServer();
    return s;
}

bool BotServer::Listen()
{
    auto port = static_cast<unsigned short>(BOTCONFIG->Value(BotConfig::ListenPort).toInt());
    BOTLOG("port:" << port);
    return server->listen(QHostAddress::Any, port);
}

void BotServer::Close()
{
    for(auto client: tcpClient)
    {
        client->disconnectFromHost();
        if(!client->waitForDisconnected(1000))
        {
            //处理异常
        }
    }
    tcpClient.clear();

    server->close();
}

BotServer::BotServer()
{
    server = std::make_shared<QTcpServer>();
    connect(server.get(), &QTcpServer::newConnection, this, &BotServer::on_newConnection);
    connect(BOTSTORE, &BotStore::NewMessage, this, &BotServer::OnNewMessage);
}

void BotServer::OnNewMessage(std::shared_ptr<BotMessage> message)
{
    for (int i = 0; i< messageIds.length(); ++i) {
        if(messageIds[i] == message->id){
            BotMessage msgForSend;
            msgForSend.roomId = message->roomId;

            auto text = message->text;
            if(message->roomType == "group"){
                auto displayName = BOTCONFIG->Value(BotConfig::BotDisplayName).toString();
                text = message->text.mid(displayName.count());
            }
            if(text.contains("picture")){// remove(' ') == "sendmepicture"){
                msgForSend.files.push_back("fengqiuhuang.png");
            }else {
                msgForSend.text = text;
            }


            NETMANAGER->sendCreateMessage(msgForSend);
            messageIds.removeAll(message->id);
        }
    }
}

void BotServer::on_newConnection()
{
    BOTLOG("New Connection!");
    currentClient.reset(server->nextPendingConnection());
    tcpClient.append(currentClient);

    connect(currentClient.get(), &QTcpSocket::readyRead, this, &BotServer::on_readyRead);
    connect(currentClient.get(), &QTcpSocket::disconnected, this, &BotServer::on_disconnected);
}

void BotServer::on_readyRead()
{

    for(int i=0; i<tcpClient.length(); i++)
    {
        QByteArray buffer = tcpClient[i]->readAll();
        if(buffer.isEmpty())
            continue;

        auto arrayList = buffer.split('\n');
        auto lastArray = arrayList.end() - 1;

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(*lastArray, &jsonError));
        BOTLOG(QString("Json parsing result:") << jsonError.error);
        if(jsonError.error != QJsonParseError::ParseError::NoError){
            continue;
        }

        QJsonObject jsonObject = jsonDoc.object();

        auto dataObject = jsonObject.value("data").toObject();
        BOTLOG(dataObject);
        auto personId = dataObject.value("personId").toString();
        if(personId == BOTCONFIG->Value(BotConfig::BotId)){
            BOTLOG("My message!");
            continue;
        }


        auto roomId = dataObject.value("roomId").toString();
        auto messageId = dataObject.value("id").toString();
        messageIds.push_back(messageId);

        NETMANAGER->sendGetMessageDetails(messageId);
        continue;
    }
}
void BotServer::on_disconnected()
{
    for(int i=0; i<tcpClient.length(); i++)
    {
        if(tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在tcpClient列表中的客户端信息
            tcpClient.removeAt(i);
        }
    }
}
