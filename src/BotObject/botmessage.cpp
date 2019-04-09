#include "botmessage.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

BotMessage::PTR BotMessage::New(QJsonObject *jsonObject)
{
    PTR ptr(new BotMessage);
    ptr->InitByJson(jsonObject);
    return ptr;
}

BotMessage::BotMessage(QObject *parent): BotObject (parent)
{

}

void BotMessage::InitByJson(QJsonObject *jsonObject)
{
    if(jsonObject)
    {
        this->id = jsonObject->value("id").toString();
        this->roomId = jsonObject->value("roomId").toString();
        this->roomType = jsonObject->value("roomType").toString();
        this->toPersonId = jsonObject->value("toPersonId").toString();
        this->toPersonEmail = jsonObject->value("toPersonEmail").toString();
        this->text = jsonObject->value("text").toString();
        this->markdown = jsonObject->value("markdown").toString();
        this->html = jsonObject->value("html").toString();
        {
            auto jsonArray = jsonObject->value("files").toArray();
            for (auto sub: jsonArray) {
                this->files.append(sub.toString());
            }

        }
        this->personId = jsonObject->value("personId").toString();
        this->personEmail = jsonObject->value("personEmail").toString();
        {
            auto jsonArray = jsonObject->value("mentionedPeople").toArray();
            for (auto sub: jsonArray) {
                this->mentionedPeople.append(sub.toString());
            }

        }
        {
            auto jsonArray = jsonObject->value("mentionedGroups").toArray();
            for (auto sub: jsonArray) {
                this->mentionedGroups.append(sub.toString());
            }

        }
        this->created = jsonObject->value("created").toString();
    }
}

QString BotMessage::Describe()
{
    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);
    ts << "\n[BotMessage]\n{"
       << "\n    id:" << id
       << "\n    roomId:" << roomId
       << "\n    roomType:" << roomType
       << "\n    toPersonId:" << toPersonId
       << "\n    toPersonEmail:" << toPersonEmail
       << "\n    text:" << text
       << "\n    markdown:" << markdown
       << "\n    html:" << html
       << "\n    files:" << files.join(",")
       << "\n    personId:" << personId
       << "\n    personEmail:" << personEmail
       << "\n    mentionedPeople:" << mentionedPeople.join(",")
       << "\n    mentionedGroups:" << mentionedGroups.join(",")
       << "\n    created:" << created
       << "\n}";
    return str;
}
