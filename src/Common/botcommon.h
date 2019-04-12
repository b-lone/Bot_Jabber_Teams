#ifndef BOTCOMMON_H
#define BOTCOMMON_H

#include <QDebug>
#include <memory>

#define BOTLOG(arg) qDebug() << "[" << __FILE__ << "][" << __LINE__ << "][" << __func__ << "]:" << arg

#define ParseJsonToString(key) key = jsonObject->value(#key).toString()
#define ParseJsonToObject(key) QJsonObject key##Object = jsonObject->value(#key).toObject()

class QJsonObject;
class QByteArray;
bool ByteArrayToJson(std::shared_ptr<QByteArray> bytes, QJsonObject * jsonObject);
bool ByteArrayToJson(const QByteArray & bytes, QJsonObject * jsonObject);

#endif // BOTCOMMON_H
