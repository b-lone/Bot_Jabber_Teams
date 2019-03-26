#include "botpeople.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

BotPeople::BotPeople(QJsonObject *jsonObject)
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

QDebug operator <<(QDebug qd, BotPeople & people)
{

    qd = qd << "\n[BotPeople]\n{"
            << "\n    id:" << people.id
            << "\n    emails:" << people.emails
            << "\n    phoneNumber:{";
    for (auto phoneNumber: people.phoneNumbers) {
        qd = qd << "\n                     type:" << phoneNumber.type
                << "\n                     value:" << phoneNumber.value
                << "\n                }";
    };
    return qd << "\n    displayName:" << people.displayName
              << "\n    nickName:" << people.nickName
              << "\n    firstName:" << people.firstName
              << "\n    lastName:" << people.lastName
              << "\n    avatar:" << people.avatar
              << "\n    orgId:" << people.orgId
              << "\n    roles:" << people.roles
              << "\n    licenses:" << people.licenses
              << "\n    created:" << people.created
              << "\n    timezone:" << people.timezone
              << "\n    lastActivity:" << people.lastActivity
              << "\n    status:" << people.status
              << "\n    invitePending:" << people.invitePending
              << "\n    loginEnabled:" << people.loginEnabled
              << "\n    type:" << people.type
              << "\n}";
}
