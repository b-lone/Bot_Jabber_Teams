#include "botwebhooksdialog.h"
#include "ui_botwebhooksdialog.h"

#include <QMessageBox>

#include "botstore.h"
#include "botwebhook.h"
#include "botnetworkmanager.h"
#include "botcommon.h"

static int columnId = 0;
static int columnTargetUrl = 1;
static int columnStatus = 2;

BotWebhooksDialog::BotWebhooksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BotWebhooksDialog)
{
    ui->setupUi(this);

    UpdateTable();
    on_tableWebhooks_itemSelectionChanged();

    connect(BOTSTORE, &BotStore::WebhookReady, this, &BotWebhooksDialog::UpdateTable);
}

BotWebhooksDialog::~BotWebhooksDialog()
{
    delete ui;
}

void BotWebhooksDialog::UpdateTable()
{
    BOTLOG("Update Table");
    auto table = ui->tableWebhooks;

    table->setRowCount(0);

    auto webhooks = BOTSTORE->getWebhooks();
    for (auto webhook : webhooks) {
        table->insertRow(0);
        table->setItem(0, columnId, new QTableWidgetItem(webhook->id));
        table->setItem(0, columnTargetUrl, new QTableWidgetItem(webhook->targetUrl));
        table->setItem(0, columnStatus, new QTableWidgetItem(webhook->status));
    }
}

void BotWebhooksDialog::on_btnRefresh_clicked()
{
    BOTLOG("Refresh Webhooks");
    BOTNETWORKMANAGER->sendListWebhooks();
}

void BotWebhooksDialog::on_btnActive_clicked()
{
    BOTLOG("Active webhook");
    auto selectedRows = ui->tableWebhooks->selectionModel()->selectedRows();
    auto id = ui->tableWebhooks->item(selectedRows[0].row(), columnStatus)->text();
    auto webhookptr = BOTSTORE->getWebhookById(id);
    if(webhookptr){
        BOTNETWORKMANAGER->sendUpdateWebhook(id, webhookptr->name, webhookptr->targetUrl);
    }
}

void BotWebhooksDialog::on_btnDelete_clicked()
{
    BOTLOG("Delete webhook");
    auto selectedRows = ui->tableWebhooks->selectionModel()->selectedRows();
    auto id = ui->tableWebhooks->item(selectedRows[0].row(), columnId)->text();
    BOTLOG(id);
    BOTNETWORKMANAGER->sendDelete(RequestType::webhooks,id);
}

void BotWebhooksDialog::on_btnCreate_clicked()
{

}

void BotWebhooksDialog::on_btnGetUrl_clicked()
{

}

void BotWebhooksDialog::on_tableWebhooks_itemSelectionChanged()
{
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
