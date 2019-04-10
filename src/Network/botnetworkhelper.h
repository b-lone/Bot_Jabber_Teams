#ifndef BOTNETWORKHELPER_H
#define BOTNETWORKHELPER_H

#include <QObject>
#include <memory>
#include <QMap>
class QNetworkReply;
class QNetworkRequest;
class QHttpMultiPart;

enum RequestType : unsigned char{
    memberships = 0,
    messages,
    people,
    rooms,
    webhooks
};
QString RequestTypeToString(RequestType type);

enum SendType{
    Get = 0,
    Post,
    Put,
    Delete,
};

class BotNetworkReplyHelper : public QObject
{
    Q_OBJECT
public:
    BotNetworkReplyHelper(QNetworkReply * nr);
    QNetworkReply * GetNetworkReply(){ return networkReply; }
    void setRequestType(RequestType type) { requestType = type; }
    RequestType getRequestType() { return requestType; }

signals:
    void finished(BotNetworkReplyHelper * nrh);

private slots:
    void on_finished();

private:
    QNetworkReply * networkReply;
    RequestType requestType;

};

class BotNetworkRepquestHelper : public QObject
{
    Q_OBJECT
public:
    typedef std::shared_ptr<BotNetworkRepquestHelper> PTR;
    static PTR New(RequestType rt, SendType st);

    BotNetworkRepquestHelper(RequestType rt, SendType st);

    std::shared_ptr<QNetworkRequest> GenerateRequest();

    void setParams(const QString &key, const QString &value) { params[key] = value; }

    void setContentType(const QString & contentType);
    QString getContentType();

    void setObjectId(const QString & Id) { objectId = Id; }

    void setRequestType(RequestType type) { requestType = type; }
    RequestType getRequestType() { return requestType; }

    void setBodyByte(const QJsonObject & jsonObject);
    const QByteArray & getBodyByte() { return bodyByte; }

    void setBodyMutiPart(QHttpMultiPart * multipart) { bodyMultiPart = multipart; }
    QHttpMultiPart * getBodyMutiPart() { return bodyMultiPart; }

    void setSendType(SendType type) { sendType = type; }
    SendType getSendType() { return  sendType; }

private:
    std::shared_ptr<QNetworkRequest> request;
    QMap<QString, QString> params;
    RequestType requestType;
    QString objectId;
    QByteArray bodyByte;
    QHttpMultiPart * bodyMultiPart;
    SendType sendType;

public:
    static const QString contentType_Json;
    static const QString mUrl;
};

#endif // BOTNETWORKHELPER_H
