#include <QApplication>

#include "botmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BotMainWindow mw;
    mw.show();

    return app.exec();
}
