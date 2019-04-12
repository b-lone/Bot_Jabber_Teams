#ifndef BOTLOCALSETTING_H
#define BOTLOCALSETTING_H

#include <QDialog>

namespace Ui {
class BotLocalSetting;
}

class BotLocalSetting : public QDialog
{
    Q_OBJECT

public:
    explicit BotLocalSetting(QWidget *parent = nullptr);
    ~BotLocalSetting();

private slots:
    void on_btnReset_clicked();
    void on_btnOK_clicked();

private:
    void Init();

private:
    Ui::BotLocalSetting *ui;
};

#endif // BOTLOCALSETTING_H
