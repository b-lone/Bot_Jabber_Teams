#ifndef BOTMESSAGE_H
#define BOTMESSAGE_H

//#include <QString>
#include <QStringList>

class QJsonObject;
//class QDebug;

class BotMessage
{
public:
    //init by json
    BotMessage() = default;
    BotMessage(QJsonObject *jsonObject);
    friend QDebug operator <<(QDebug qd, BotMessage & message);

    QString id;
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

QDebug operator <<(QDebug qd, BotMessage & message);
#endif // BOTMESSAGE_H
