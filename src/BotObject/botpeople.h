#ifndef BOTPEOPLE_H
#define BOTPEOPLE_H
#include <botobject.h>

#include <QVector>
#include <QStringList>
class QJsonObject;

struct BotPhoneNumber
{
    QString type;
    QString value;
};

class BotPeople : public BotObject
{
public:
    typedef std::shared_ptr<BotPeople> PTR;
    static PTR New(QJsonObject * jsonObject);

    //init by json
    BotPeople(QObject *parent = nullptr);

    void InitByJson(QJsonObject * jsonObject);

    virtual QString Describe();

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

#endif // BOTPEOPLE_H
