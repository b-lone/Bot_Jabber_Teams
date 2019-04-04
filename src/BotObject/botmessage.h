#ifndef BOTMESSAGE_H
#define BOTMESSAGE_H

#include <botobject.h>

#include <QStringList>

class QJsonObject;
//class QDebug;

class BotMessage : public BotObject
{
public:
    //init by json
    BotMessage() = default;
    BotMessage(QJsonObject *jsonObject);

    virtual QString Describe();

    QString roomId;
    QString roomType;
    QString toPersonId;
    QString toPersonEmail;
    QString text;
    QString markdown;
    QString html;
    QStringList files;
    QString personId;
    QString personEmail;
    QStringList mentionedPeople;
    QStringList mentionedGroups;
    QString created;
};

#endif // BOTMESSAGE_H
