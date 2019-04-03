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

BotTcpSocketHelper::BotTcpSocketHelper(QTcpSocket *ts): QObject (ts)
{
    tcpSocket = std::shared_ptr<QTcpSocket>(ts);
    connect(ts, &QTcpSocket::readyRead, this, &BotTcpSocketHelper::on_readyRead);
    connect(ts, &QTcpSocket::disconnected, this, &BotTcpSocketHelper::on_disconnected);
}

void BotTcpSocketHelper::on_readyRead()
{
    emit readyRead(this);
}

void BotTcpSocketHelper::on_disconnected()
{
    emit disconnected(this);
}

//--------------------------------------------------------------------------------------------
BotServer *BotServer::Instance()
{
    static BotServer * s = new BotServer();
    return s;
}

bool BotServer::Listen()
{
    bool result = true;

    {
        auto portNgrok = static_cast<unsigned short>(BOTCONFIG->Value(BotConfig::ngrokPort).toInt());
        BOTLOG("ngrok port:" << portNgrok);
        if(! serverNgrok->listen(QHostAddress::Any, portNgrok)){
            result = false;
        }
    }
    {
        auto portAutomation = static_cast<unsigned short>(BOTCONFIG->Value(BotConfig::automationPort).toInt());
        BOTLOG("automation port:" << portAutomation);
        if(! serverAutomation->listen(QHostAddress::Any, portAutomation)){
            result = false;
        }
    }

    return result;
}

void BotServer::Close()
{
    {
        for(auto client: tcpClientNgrok)
        {
            client->disconnectFromHost();
            if(!client->waitForDisconnected(1000))
            {
                //处理异常
            }
        }
        tcpClientNgrok.clear();
        serverNgrok->close();
    }
    {
        for(auto client: tcpClientAutomation)
        {
            client->disconnectFromHost();
            if(!client->waitForDisconnected(1000))
            {
                //处理异常
            }
        }
        tcpClientAutomation.clear();
        serverAutomation->close();
    }
}

BotServer::BotServer()
{
    serverNgrok = std::make_shared<QTcpServer>();
    connect(serverNgrok.get(), &QTcpServer::newConnection, this, &BotServer::on_newConnectionNgrok);
    connect(BOTSTORE, &BotStore::MessageReady, this, &BotServer::OnNewMessage);

    serverAutomation = std::make_shared<QTcpServer>();
    connect(serverAutomation.get(), &QTcpServer::newConnection, this, &BotServer::on_newConnectionAutomation);
}

void BotServer::OnNewMessage(std::shared_ptr<BotMessage> message)
{
    BOTLOG("New Message");
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
                msgForSend.files.push_back("picture/hello.png");
            }else {
                msgForSend.text = text;
            }


            NETMANAGER->sendCreateMessage(msgForSend);
            messageIds.removeAll(message->id);
        }
    }
}

void BotServer::on_newConnectionNgrok()
{
    BOTLOG("New Connection!");
//    std::shared_ptr<QTcpSocket> currentClient(serverNgrok->nextPendingConnection());
//    tcpClientNgrok.append(currentClient);

//    BotTcpSocketHelper * tsh = new BotTcpSocketHelper(currentClient.get());;
//    connect(tsh, &BotTcpSocketHelper::readyRead, this, &BotServer::on_readyReadNgrok);
//    connect(tsh, &BotTcpSocketHelper::disconnected, this, &BotServer::on_disconnectedNgrok);
    auto currentClient = serverNgrok->nextPendingConnection();
    tcpClientNgrok.append(currentClient);

    connect(currentClient, &QTcpSocket::readyRead, this, &BotServer::on_readyReadNgrok);
    connect(currentClient, &QTcpSocket::disconnected, this, &BotServer::on_disconnectedNgrok);
}

void BotServer::on_readyReadNgrok()
{
    BOTLOG("New Webhook Push.");

    for (int i = 0; i < tcpClientNgrok.count(); ++i) {
        QByteArray buffer = tcpClientNgrok[i]->readAll();
        if(buffer.isEmpty())
            continue;

        auto arrayList = buffer.split('\n');
        auto lastArray = arrayList.end() - 1;

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(*lastArray, &jsonError));
        BOTLOG(QString("Json parsing result:") << jsonError.error);
        if(jsonError.error != QJsonParseError::ParseError::NoError){
            return;
        }

        QJsonObject jsonObject = jsonDoc.object();

        auto dataObject = jsonObject.value("data").toObject();
        BOTLOG(dataObject);
        auto personId = dataObject.value("personId").toString();
        if(personId == BOTCONFIG->Value(BotConfig::BotId)){
            BOTLOG("My message!");
            return;
        }else {
            auto roomId = dataObject.value("roomId").toString();
            auto messageId = dataObject.value("id").toString();
            messageIds.push_back(messageId);

            NETMANAGER->sendGetMessageDetails(messageId);
        }
    }
//    auto tcpClient = nrh->GetTcpSocket();
//    BOTLOG("1");

//    QByteArray buffer = tcpClient->readAll();
//    if(buffer.isEmpty())
//        return;

//    auto arrayList = buffer.split('\n');
//    auto lastArray = arrayList.end() - 1;

//    QJsonParseError jsonError;
//    QJsonDocument jsonDoc(QJsonDocument::fromJson(*lastArray, &jsonError));
//    BOTLOG(QString("Json parsing result:") << jsonError.error);
//    if(jsonError.error != QJsonParseError::ParseError::NoError){
//        return;
//    }

//    QJsonObject jsonObject = jsonDoc.object();

//    auto dataObject = jsonObject.value("data").toObject();
//    BOTLOG(dataObject);
//    auto personId = dataObject.value("personId").toString();
//    if(personId == BOTCONFIG->Value(BotConfig::BotId)){
//        BOTLOG("My message!");
//        return;
//    }else {
//        auto roomId = dataObject.value("roomId").toString();
//        auto messageId = dataObject.value("id").toString();
//        messageIds.push_back(messageId);

//        NETMANAGER->sendGetMessageDetails(messageId);
//    }

}
void BotServer::on_disconnectedNgrok()
{
    BOTLOG("disconnected.");
//    tcpClientNgrok.removeOne(nrh->GetTcpSocket());
    for(int i=0; i<tcpClientNgrok.length(); i++)
    {
        if(tcpClientNgrok[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在tcpClient列表中的客户端信息
            tcpClientNgrok.removeAt(i);
        }
    }
}

void BotServer::on_newConnectionAutomation()
{
    BOTLOG("New Connection!");
    std::shared_ptr<QTcpSocket> currentClient(serverAutomation->nextPendingConnection());
    tcpClientAutomation.append(currentClient);

    BotTcpSocketHelper * tsh = new BotTcpSocketHelper(currentClient.get());
    connect(tsh, &BotTcpSocketHelper::readyRead, this, &BotServer::on_readyReadAutomation);
    connect(tsh, &BotTcpSocketHelper::disconnected, this, &BotServer::on_disconnectedNgrok);
}

void BotServer::on_readyReadAutomation(BotTcpSocketHelper *nrh)
{
    auto tcpClient = nrh->GetTcpSocket();

    QByteArray buffer = tcpClient->readAll();
    if(buffer.isEmpty())
        return;
    BOTLOG("buffer");
}

void BotServer::on_disconnectedAutomation(BotTcpSocketHelper *nrh)
{
    tcpClientAutomation.removeOne(nrh->GetTcpSocket());
}
