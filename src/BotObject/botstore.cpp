#include "botstore.h"

#include "botmessage.h"
#include "botroom.h"
#include "botmembership.h"
#include "botpeople.h"
#include "botwebhook.h"
#include "botcommon.h"

BotStore *BotStore::Instance()
{
    static BotStore * store = new BotStore();
    return store;
}

BotStore::BotStore(QObject *parent) : QObject(parent)
{

}

void BotStore::PushMessage(std::shared_ptr<BotMessage> object)
{
    if (std::find_if(messagesStore.begin(), messagesStore.end(), [&](std::shared_ptr<BotMessage> item){return (item->id == object->id);}) == messagesStore.end()) {
        BOTLOG("Add new object from:" << object->personId);
        messagesStore.push_back(object);
        emit MessageReady(object);
    }else {
        BOTLOG("Already exist!");
    }
}

void BotStore::PushRoom(std::shared_ptr<BotRoom> object)
{
    if (std::find_if(roomStore.begin(), roomStore.end(), [&](std::shared_ptr<BotRoom> item){return (item->id == object->id);}) == roomStore.end()) {
        BOTLOG("Add new object:" << object->title);
        roomStore.push_back(object);
        emit RoomReady(object);
    }else {
        BOTLOG("Already exist!");
    }
}

void BotStore::PushMembership(std::shared_ptr<BotMembership> object)
{
    if (std::find_if(membershipStore.begin(), membershipStore.end(), [&](std::shared_ptr<BotMembership> item){return (item->id == object->id);}) == membershipStore.end()) {
        BOTLOG("Add new object:" << object->personDisplayName);
        membershipStore.push_back(object);
        emit MembershipReady(object);
    }else {
        BOTLOG("Already exist!");
    }
}

void BotStore::PushPeople(std::shared_ptr<BotPeople> object)
{
    if (std::find_if(peopleStore.begin(), peopleStore.end(), [&](std::shared_ptr<BotPeople> item){return (item->id == object->id);}) == peopleStore.end()) {
        BOTLOG("Add new object:" << object->displayName);
        peopleStore.push_back(object);
        emit PeopleReady(object);
    }else {
        BOTLOG("Already exist!");
    }
}

void BotStore::PushWebhook(std::shared_ptr<BotWebhook> object)
{
    if (std::find_if(webhookStore.begin(), webhookStore.end(), [&](std::shared_ptr<BotWebhook> item){return (item->id == object->id);}) == webhookStore.end()) {
        BOTLOG("Add new object:" << object->name);
        webhookStore.push_back(object);
        emit WebhookReady(object);
    }else {
        BOTLOG("Already exist!");
    }
}
