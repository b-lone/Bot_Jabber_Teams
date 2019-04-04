#include "botobject.h"

#include <QDebug>

BotObject::BotObject(QObject *parent) : QObject(parent)
{

}

QDebug operator <<(QDebug qd, BotObject &object)
{
    return qd << object.Describe();
}
