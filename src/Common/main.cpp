#include <QApplication>
#include <QDebug>

#include "botmainwindow.h"

int main(int argc, char *argv[])
{
//    int exitCode = 0;
//    do{
//        QApplication app(argc, argv);

//        BotMainWindow mw;
//        mw.show();

//        exitCode = app.exec();
//    }while (exitCode == BotMainWindow::exitCodeReboot);

    QApplication app(argc, argv);

    BotMainWindow mw;
    mw.show();

    return app.exec();
}
