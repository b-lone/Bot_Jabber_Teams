#include "botserver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "botconfig.h"
#include "botcommon.h"
#include "botnetworkmanager.h"
#include "botmessage.h"

BotServer *BotServer::Instance()
{
    static BotServer * s = new BotServer();
    return s;
}

bool BotServer::Listen()
{
    auto port = static_cast<unsigned short>(CONFIG->Value(BotConfig::ListenPort).toInt());
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
            return;
        }

        QJsonObject jsonObject = jsonDoc.object();

        auto dataObject = jsonObject.value("data").toObject();
        auto roomId = dataObject.value("roomId").toString();
        auto messageId = dataObject.value("id");
        BotMessage message;
        message.roomId = roomId;
        message.text = "Hi";
        NETMANAGER->sendCreateMessage(message);


        qDebug() << jsonObject;
//        static QString IP_Port, IP_Port_Pre;
//        IP_Port = tr("[%1:%2]:").arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
//                .arg(tcpClient[i]->peerPort());

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
