#include "botcommon.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>


bool ParseBytesToJson(const QByteArray &bytes, QJsonObject *jsonObject)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &jsonError));
    BOTLOG(QString("Json parsing result:") << jsonError.error);
    if(jsonError.error != QJsonParseError::ParseError::NoError){
        return false;
    }
    *jsonObject = jsonDoc.object();
    return true;
}
