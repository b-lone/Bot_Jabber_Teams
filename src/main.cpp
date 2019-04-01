#include <thread>
#include <QApplication>
#include <QDebug>
#include "botcommon.h"

#include "botmainwindow.h"

int main(int argc, char *argv[])
{
    BOTLOG("begin");
    QApplication app(argc, argv);

    BotMainWindow mw;
    mw.show();

    return app.exec();
}
