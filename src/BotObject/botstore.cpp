#include "botstore.h"

#include "botmessage.h"
#include "botroom.h"
#include "botcommon.h"

BotStore *BotStore::Instance()
{
    static BotStore * store = new BotStore();
    return store;
}

BotStore::BotStore(QObject *parent) : QObject(parent)
{

}

void BotStore::AddNewMessage(std::shared_ptr<BotMessage> message)
{
    if (std::find_if(messagesStore.begin(), messagesStore.end(), [&](std::shared_ptr<BotMessage> item){return (item->id == message->id);}) == messagesStore.end()) {
        BOTLOG("Add new message from:" << message->personId);
        messagesStore.push_back(message);
        emit NewMessage(message);
    }else {
        BOTLOG("Mssage already exist!");
    }
}
