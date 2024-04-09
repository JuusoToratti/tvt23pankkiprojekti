#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


accWithdrawals::accWithdrawals(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accWithdrawals)
{
    ui->setupUi(this);

    connect(ui->withBackButton, &QPushButton::clicked, this, &accWithdrawals::handleBack);
}

accWithdrawals::~accWithdrawals()
{
    delete ui;
}

void accWithdrawals::handleBack()
{
    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow3 = new mainUserInterface();
    mainUserInterfaceWindow3->show();

    //suljetaan nykyinen ikkuna
    this->close();
}
