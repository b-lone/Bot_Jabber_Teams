#ifndef BOTMAINWINDOW_H
#define BOTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class BotMainWindow;
}

class BotMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BotMainWindow(QWidget *parent = nullptr);
    ~BotMainWindow();

private slots:
    void on_btnSend_clicked();

private:
    Ui::BotMainWindow *ui;
};

#endif // BOTMAINWINDOW_H
