#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


accWithdrawals::accWithdrawals(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accWithdrawals)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->withBackButton, &QPushButton::clicked, this, &accWithdrawals::handleBack);
}

accWithdrawals::~accWithdrawals()
{
    delete ui;
}

void accWithdrawals::handleBack()
{
    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();
    mainUserInterfaceWindow->show();

    //suljetaan nykyinen ikkuna
    this->close();
}
