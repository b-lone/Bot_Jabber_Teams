#include <thread>
#include <QApplication>
#include <QProcess>

#include "botmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BotMainWindow mw;
    mw.show();

    return app.exec();
}
