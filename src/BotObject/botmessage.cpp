#include "botmessage.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

BotMessage::BotMessage(QJsonObject *jsonObject)
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

QDebug operator <<(QDebug qd, BotMessage &message)
{
    return qd << "\n[BotMessage]\n{"
              << "\n    id:" << message.id
              << "\n    roomId:" << message.roomId
              << "\n    roomType:" << message.roomType
              << "\n    toPersonId:" << message.toPersonId
              << "\n    toPersonEmail:" << message.toPersonEmail
              << "\n    text:" << message.text
              << "\n    markdown:" << message.markdown
              << "\n    files:" << message.files
              << "\n    personId:" << message.personId
              << "\n    personEmail:" << message.personEmail
              << "\n    mentionedPeople:" << message.mentionedPeople
              << "\n    mentionedGroups:" << message.mentionedGroups
              << "\n    created:" << message.created
              << "\n}";
}
