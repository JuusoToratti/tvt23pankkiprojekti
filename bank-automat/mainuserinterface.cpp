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
    connect(ui->showTransactions, QPushButton::clicked, this, &mainUserInterface::handleWithClicked);
    connect(ui->showBalance, QPushButton::clicked, this , &mainUserInterface::getCreditBalanceSlot);
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

void mainUserInterface::handleWithClicked()
{
    accWithdrawals *accWithdrawalsWindow = new accWithdrawals();
    accWithdrawalsWindow->show();

    // Suljetaan mainUserInterface-ikkuna
    this->close();
}

void mainUserInterface::getCreditBalanceSlot()
{
    //jos rest-api puolella url muuttuu, niin tähän myös muutos
    QString site_url="http://localhost:3000/accounts/getaccountbalance";
    QNetworkRequest request((site_url));

    pgetManager = new QNetworkAccessManager(this);
    connect(pgetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkRequestFinished(QNetworkReply*)));
    preply = pgetManager->get(request);
    //WEBTOKEN ALKU
    //QByteArray myToken="Bearer "+webToken;
    //request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN LOPPU
}

void mainUserInterface::onNetworkRequestFinished(QNetworkReply *preply)
{
    QByteArray response_data=preply->readAll();
    qDebug()<<"DATA : "+response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();
    //QString idaccount=json_array.at(0)["idaccount"].toString();


    QString cCredit;

    /*for (const QJsonValue &value : json_array) {
        QJsonObject json_obj = value.toObject();
        int idaccount = json_obj["idaccount"].toInt();
        if (idaccount == 1) {
            cCredit = json_obj["account_balance"].toString();
            break;
        }
    }*/

    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
        if (json_obj["idaccount"].toInt() == 1) { // Tarkistetaan idaccountin arvo
            QString account_balance = json_obj["account_balance"].toString();
            cCredit = account_balance;
            qDebug() << "Account balance: " << cCredit;
            break; // Keskeytetään silmukka, kun haluttu käyttäjä löytyy
            //cName+=QString::number(json_obj["iduser"].toInt())+", "+json_obj["fname"].toString()+", "+json_obj["lname"].toString()+"\r";
        }
    }

    // Luo uusi ikkuna ja käyttöliittymäolio
    accountBalance *accountBalanceWindow = new accountBalance();

    QLabel *creditLabel = accountBalanceWindow->findChild<QLabel*>("creditLabel");
    if (creditLabel) {
        creditLabel->setText(cCredit);
    }
    accountBalanceWindow->show();

    //suljetaan nykyinen ikkuna
    this->close();

    preply->deleteLater();
    pgetManager->deleteLater();
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
