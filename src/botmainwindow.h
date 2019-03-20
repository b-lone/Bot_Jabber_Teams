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
    void on_btnRooms_clicked();

    void on_btnMemberships_clicked();

    void on_btnAddMem_clicked();

private:
    Ui::BotMainWindow *ui;
};

#endif // BOTMAINWINDOW_H
