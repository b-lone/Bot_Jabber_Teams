#ifndef BOTPEOPLE_H
#define BOTPEOPLE_H

#include <QVector>
#include <QStringList>
class QJsonObject;
//class QDebug;

struct BotPhoneNumber
{
    QString type;
    QString value;
};

class BotPeople
{
public:
    //init by json
    BotPeople(QJsonObject *jsonObject);

    friend QDebug operator <<(QDebug os, BotPeople & people);

    QString id;
    QStringList emails;
    QVector<BotPhoneNumber> phoneNumbers;
    QString displayName;
    QString nickName;
    QString firstName;
    QString lastName;
    QString avatar;
    QString orgId;
    QStringList roles;
    QStringList licenses;
    QString created;
    QString timezone;
    QString lastActivity;
    QString status;
    QString invitePending;
    QString loginEnabled;
    QString type;
};

QDebug operator <<(QDebug qd, BotPeople & people);

#endif // BOTPEOPLE_H
