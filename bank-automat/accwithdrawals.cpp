#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>

accWithdrawals::accWithdrawals(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accWithdrawals)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->withBackButton, &QPushButton::clicked, this, &accWithdrawals::handleBack);
    connect(ui->deleteActionsButton, &QPushButton::clicked, this, &accWithdrawals::clearAccountEvents);
}

accWithdrawals::~accWithdrawals()
{
    delete ui;
}

void accWithdrawals::transTableWidget(QStandardItemModel *model)
{
    ui->transTableWidget->setModel(model);
}

void accWithdrawals::handleBack()
{
    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();
    mainUserInterfaceWindow->show();

    //suljetaan nykyinen ikkuna
    this->close();
}

void accWithdrawals::clearAccountEvents()
{
    qDebug() << "Nostot tyhjennetty";

    // Tyhjennä transTableWidget
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->transTableWidget->model());
    if (model) {
        model->removeRows(1, model->rowCount() - 1);
    }
}
