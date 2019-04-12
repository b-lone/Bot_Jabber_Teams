#include "botwebhooksdialog.h"
#include "ui_botwebhooksdialog.h"

#include <QMessageBox>

#include "botstore.h"
#include "botwebhook.h"
#include "bothttpclient.h"
#include "bothttpclientcontroller.h"
#include "botcommon.h"

static int columnId = 0;
static int columnTargetUrl = 1;
static int columnStatus = 2;
static int columnResouce = 3;
static int columnEvent = 4;

BotWebhooksDialog::BotWebhooksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BotWebhooksDialog)
{
    ui->setupUi(this);

    UpdateTable();
    on_tableWebhooks_itemSelectionChanged();
    ui->btnCreate->setEnabled(false);

    connect(S_STORE, &BotStore::webhookReady, this, &BotWebhooksDialog::UpdateTable);
    connect(S_HTTPCLIENT, &BotHttpClient::readyWithoutData, this, &BotWebhooksDialog::on_deleteFinshed);
    auto networkController = S_HTTPCLIENT->getNetworkController();
    connect(networkController, &BotHttpClientController::ngrokUrlReady, this, &BotWebhooksDialog::update_leUrl);
}

BotWebhooksDialog::~BotWebhooksDialog()
{
    BOTLOG("~BotWebhooksDialog");
    delete ui;
}

void BotWebhooksDialog::UpdateTable()
{
    BOTLOG("Update Table");
    auto table = ui->tableWebhooks;

    table->setRowCount(0);

    auto webhooks = S_STORE->getWebhooks();
    for (auto webhook : webhooks) {
        table->insertRow(0);
        table->setItem(0, columnId, new QTableWidgetItem(webhook->id));
        table->setItem(0, columnTargetUrl, new QTableWidgetItem(webhook->targetUrl));
        table->setItem(0, columnStatus, new QTableWidgetItem(webhook->status));
        table->setItem(0, columnResouce, new QTableWidgetItem(webhook->resource));
        table->setItem(0, columnEvent, new QTableWidgetItem(webhook->event));
    }
}

void BotWebhooksDialog::on_btnRefresh_clicked()
{
    BOTLOG("Refresh Webhooks");
    S_HTTPCLIENT->sendListWebhooks();
}

void BotWebhooksDialog::on_btnActive_clicked()
{
    BOTLOG("Active webhook");
    auto selectedRows = ui->tableWebhooks->selectionModel()->selectedRows();
    auto id = ui->tableWebhooks->item(selectedRows[0].row(), columnStatus)->text();
    auto webhookptr = S_STORE->getWebhookById(id);
    if(webhookptr){
        S_HTTPCLIENT->sendUpdateWebhook(id, webhookptr->name, webhookptr->targetUrl);
    }
}

void BotWebhooksDialog::on_btnDelete_clicked()
{
    BOTLOG("Delete webhook");
    auto selectedRows = ui->tableWebhooks->selectionModel()->selectedRows();
    auto id = ui->tableWebhooks->item(selectedRows[0].row(), columnId)->text();
    BOTLOG(id);
    S_HTTPCLIENT->sendDelete(RequestType::webhooks, id);
}

void BotWebhooksDialog::on_btnCreate_clicked()
{
    BOTLOG("on_btnCreate_clicked");
    BotWebhook object;
    object.targetUrl = ui->leUrl->text();
    object.name = "Bot";
    object.resource = ui->cbResource->currentText();
    object.event = ui->cbEvent->currentText();
    BOTLOG(object);
    S_HTTPCLIENT->sendCreateWebhook(object);
}

void BotWebhooksDialog::on_btnGetUrl_clicked()
{
    BOTLOG("on_btnGetUrl_clicked");
    S_HTTPCLIENT->sendGetNgrokInfo();
}

void BotWebhooksDialog::on_tableWebhooks_itemSelectionChanged()
{
    BOTLOG("on_tableWebhooks_itemSelectionChanged");
    auto selectedRows = ui->tableWebhooks->selectionModel()->selectedRows();
    if(selectedRows.count() == 0){
        ui->btnDelete->setEnabled(false);
        ui->btnActive->setEnabled(false);
    }else {
        auto status = ui->tableWebhooks->item(selectedRows[0].row(), columnStatus)->text();
        if(status == "active"){
            ui->btnActive->setEnabled(false);
        }else {
            ui->btnActive->setEnabled(true);
        }
        ui->btnDelete->setEnabled(true);
    }
}

void BotWebhooksDialog::update_leUrl(QString Url)
{
    BOTLOG("update_leUrl");
    ui->leUrl->setText(Url);
}

void BotWebhooksDialog::on_leUrl_textChanged(const QString &arg1)
{
    if(arg1 == ""){
        ui->btnCreate->setEnabled(false);
    }else {
        ui->btnCreate->setEnabled(true);
    }
}

void BotWebhooksDialog::on_deleteFinshed(RequestType type)
{
    if(type != RequestType::webhooks){
        return;
    }
    BOTLOG("on_deleteFinshed");
    S_HTTPCLIENT->sendListWebhooks();
}
