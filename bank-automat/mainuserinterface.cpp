#include "mainuserinterface.h"
#include "ui_mainuserinterface.h"
#include "moneyselect.h"

mainUserInterface::mainUserInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainUserInterface)
{
    ui->setupUi(this);

    connect(ui->logOut, &QPushButton::clicked, this, &mainUserInterface::logoutClicked);
    connect(ui->withdrawMoney, &QPushButton::clicked, this, &mainUserInterface::withdrawMoneyClicked);
}

mainUserInterface::~mainUserInterface()
{
    delete ui;
}

void mainUserInterface::withdrawMoneyClicked()
{
    qDebug() << "withrdawMoneyClicked funktiossa";

    // Suljetaan mainUserInterface-ikkuna
    this->close();

    // Luodaan ja näytetään moneySelect-olio
    moneySelect *moneySelectWindow = new moneySelect();
    moneySelectWindow->show();
}

void mainUserInterface::logoutClicked()
{
    qDebug() << "logoutclickedissä";

    emit backToMainwindow();
}
