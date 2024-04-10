#include "mainuserinterface.h"
#include "ui_mainuserinterface.h"
#include "moneyselect.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accountbalance.h"
#include "ui_accountbalance.h"
#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include <QProcess>

mainUserInterface::mainUserInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainUserInterface)

{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->logOut, &QPushButton::clicked, this, &mainUserInterface::logoutClicked);
    connect(ui->withdrawMoney, &QPushButton::clicked, this, &mainUserInterface::withdrawMoneyClicked);
    connect(ui->showBalance, QPushButton::clicked, this, &mainUserInterface::handleBalanceClicked);
    connect(ui->showTransactions, QPushButton::clicked, this, &mainUserInterface::handleWithClicked);
}

mainUserInterface::~mainUserInterface()
{
    delete ui;
}

void mainUserInterface::withdrawMoneyClicked()
{
    qDebug() << "withrdawMoneyClicked funktiossa";

    // Luodaan ja näytetään moneySelect-olio
    moneySelect *moneySelectWindow = new moneySelect();
    moneySelectWindow->show();

    // Suljetaan mainUserInterface-ikkuna
    this->close();
}

void mainUserInterface::handleBalanceClicked()
{
    accountBalance *accountBalanceWindow = new accountBalance();
    accountBalanceWindow->show();

    // Suljetaan mainUserInterface-ikkuna
    this->close();
}

void mainUserInterface::handleWithClicked()
{
    accWithdrawals *accWithdrawalsWindow = new accWithdrawals();
    accWithdrawalsWindow->show();

    // Suljetaan mainUserInterface-ikkuna
    this->close();
}

void mainUserInterface::logoutClicked()
{
    qDebug() << "logoutclickedissä";


    // Sulje sovellus kokonaan
    qApp->quit();

    // Käynnistä sovellus uudelleen
    QProcess::startDetached(QApplication::applicationFilePath());

    //this->close();

    //Avataan pääikkuna (mainwindow.ui)
    //MainWindow *mainWindow = new MainWindow();
    //mainWindow->show();
}
