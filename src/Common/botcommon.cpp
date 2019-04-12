#include "botcommon.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>


bool ByteArrayToJson(std::shared_ptr<QByteArray> bytes, QJsonObject *jsonObject)
{
    if(!bytes)
        return false;

    return ByteArrayToJson(*bytes, jsonObject);
}

bool ByteArrayToJson(const QByteArray &bytes, QJsonObject *jsonObject)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &jsonError));

    BOTLOG(QString("Json parsing result:") << jsonError.error);
    if(jsonError.error != QJsonParseError::ParseError::NoError){
        return false;
    }

    *jsonObject =  jsonDoc.object();
    return true;
}
