#include "botpeople.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

BotPeople::PTR BotPeople::New(QJsonObject *jsonObject)
{
    PTR ptr(new BotPeople);
    ptr->InitByJson(jsonObject);
    return ptr;
}

BotPeople::BotPeople(QObject *parent): BotObject (parent)
{

}

BotPeople::BotPeople(QJsonObject *jsonObject)
{
    InitByJson(jsonObject);
}

void BotPeople::InitByJson(QJsonObject *jsonObject)
{
    if(jsonObject){
        this->id = jsonObject->value("id").toString();
        {
            auto jsonArray = jsonObject->value("emails").toArray();
            for (auto sub: jsonArray) {
                this->emails.append(sub.toString());
            }

        }
        {
            auto jsonArray = jsonObject->value("phoneNumbers").toArray();
            for (auto sub: jsonArray) {
                BotPhoneNumber phoneNumber;
                phoneNumber.type = sub.toObject().value("type").toString();
                phoneNumber.value = sub.toObject().value("value").toString();
                this->phoneNumbers.push_back(phoneNumber);
            }
        }
        this->displayName = jsonObject->value("displayName").toString();
        this->nickName = jsonObject->value("nickName").toString();
        this->firstName = jsonObject->value("firstName").toString();
        this->lastName = jsonObject->value("lastName").toString();
        this->avatar = jsonObject->value("avatar").toString();
        this->orgId = jsonObject->value("orgId").toString();
        {
            auto jsonArray = jsonObject->value("roles").toArray();
            for (auto sub: jsonArray) {
                this->roles.append(sub.toString());
            }

        }
        {
            auto jsonArray = jsonObject->value("licenses").toArray();
            for (auto sub: jsonArray) {
                this->licenses.append(sub.toString());
            }

        }
        this->created = jsonObject->value("created").toString();
        this->timezone = jsonObject->value("timezone").toString();
        this->lastActivity = jsonObject->value("lastActivity").toString();
        this->status = jsonObject->value("status").toString();
        this->invitePending = jsonObject->value("invitePending").toString();
        this->loginEnabled = jsonObject->value("loginEnabled").toString();
        this->type = jsonObject->value("type").toString();
    }
}

QString BotPeople::Describe()
{
    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);
    ts << "\n[BotPeople]\n{"
       << "\n    id:" << id
       << "\n    emails:" << emails.join(",")
       << "\n    phoneNumber:{";
    for (auto phoneNumber: phoneNumbers) {
        ts << "\n                     type:" << phoneNumber.type
           << "\n                     value:" << phoneNumber.value
           << "\n                }";
    };
    ts << "\n    displayName:" << displayName
       << "\n    nickName:" << nickName
       << "\n    firstName:" << firstName
       << "\n    lastName:" << lastName
       << "\n    avatar:" << avatar
       << "\n    orgId:" << orgId
       << "\n    roles:" << roles.join(",")
       << "\n    licenses:" << licenses.join(",")
       << "\n    created:" << created
       << "\n    timezone:" << timezone
       << "\n    lastActivity:" << lastActivity
       << "\n    status:" << status
       << "\n    invitePending:" << invitePending
       << "\n    loginEnabled:" << loginEnabled
       << "\n    type:" << type
       << "\n}";
    return str;
}
