#include "accountbalance.h"
#include "ui_accountbalance.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QCloseEvent>
#include "mainuserinterface.h"

accountBalance::accountBalance(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accountBalance)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->logoutButton2, &QPushButton::clicked, this, &accountBalance::handleLogoutClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &accountBalance::handleBackClicked);
}

accountBalance::~accountBalance()
{
    delete ui;
}

void accountBalance::handleBackClicked()
{
    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow2 = new mainUserInterface();
    mainUserInterfaceWindow2->show();

    //suljetaan nykyinen ikkuna
    this->close();
}

void accountBalance::handleLogoutClicked()
{
    // Sulje sovellus kokonaan
    qApp->quit();

    // Käynnistä sovellus uudelleen
    QProcess::startDetached(QApplication::applicationFilePath());
}
