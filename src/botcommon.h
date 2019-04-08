#ifndef BOTCOMMON_H
#define BOTCOMMON_H

#include <QDebug>
#include <memory>

#define BOTLOG(arg) qDebug() << "[" << __FILE__ << "][" << __LINE__ << "][" << __func__ << "]:" << arg

class QJsonObject;
class QByteArray;
bool ParseBytesToJson(std::shared_ptr<QByteArray> bytes, QJsonObject * jsonObject);
bool ParseBytesToJson(const QByteArray & bytes, QJsonObject * jsonObject);

#endif // BOTCOMMON_H
