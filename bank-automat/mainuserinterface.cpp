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
    connect(ui->showTransactions, QPushButton::clicked, this, &mainUserInterface::handleTransactionsClicked);
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

void mainUserInterface::handleTransactionsClicked()
{
    accWithdrawals *accWithdrawalsWindow = new accWithdrawals();
    accWithdrawalsWindow->show();

    // Suljetaan mainUserInterface-ikkuna
    this->close();
}

void mainUserInterface::getCreditBalanceSlot()
{
    //Debit balance alla

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

    //Rahanostot alla

    //jos rest-api puolella url muuttuu, niin tähän myös muutos
    QString transSite_url="http://localhost:3000/accountinformation/info";
    QNetworkRequest transRequest((transSite_url));

    transPgetManager = new QNetworkAccessManager(this);
    connect(transPgetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(showTransactionsSlot(QNetworkReply*)));
    transPreply = transPgetManager->get(transRequest);
    //WEBTOKEN ALKU
    //QByteArray myToken="Bearer "+webToken;
    //request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN LOPPU
}

void mainUserInterface::onNetworkRequestFinished(QNetworkReply *preply)
{
    qDebug()<<"onNetworkfinished funktiossa";

    QByteArray response_data=preply->readAll();
    qDebug()<<"DATA : "+response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();

    QString cCredit;

    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
         QString account_balance = json_obj["account_balance"].toString();
          cCredit = account_balance;
           qDebug() << "Account balance: " << cCredit;
            //break; // Keskeytetään silmukka, kun haluttu käyttäjä löytyy
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

void mainUserInterface::showTransactionsSlot(QNetworkReply *transPreply)
{
    qDebug()<<"showtransactions funktiossa";

    QByteArray transResponse_data=transPreply->readAll();
    qDebug()<<"DATA : "+transResponse_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(transResponse_data);
    QJsonArray json_array = json_doc.array();

    // Luo uusi ikkuna ja käyttöliittymäolio
    accountBalance *accountBalanceWindow = new accountBalance();

    QTableWidget *transTableWidget = accountBalanceWindow->findChild<QTableWidget*>("transTableWidget");

    if (transTableWidget) {

        int row = 1;

        foreach (const QJsonValue &value, json_array) {
            QJsonObject json_obj = value.toObject();

            int idAccount = json_obj["idaccount"].toInt();
            QString transactions = json_obj["transactions"].toString();
            QString amount = json_obj["amount"].toString();
            QString date = json_obj["date"].toString();

            qDebug() << "idaccount: " << idAccount;
            qDebug() << "transactions: " << transactions;
            qDebug() << "amount: " << amount;
            qDebug() << "date:  " << date;

            qDebug() << "Rows in table:" << transTableWidget->rowCount();
            qDebug() << "Columns in table:" << transTableWidget->columnCount();

        // Lisää tiedot soluihin
        qDebug() << "Adding data to row:" << row;
        transTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(idAccount)));
        transTableWidget->setItem(row, 1, new QTableWidgetItem(transactions));
        transTableWidget->setItem(row, 2, new QTableWidgetItem((amount)));
        transTableWidget->setItem(row, 3, new QTableWidgetItem(date));

        // Siirry seuraavaan riviin
        row++;
    }

    } else {
        qDebug() << "transTableWidget not found!";
    }

    transPreply->deleteLater();
    transPgetManager->deleteLater();
}

void mainUserInterface::logoutClicked()
{
    qDebug() << "logoutclickedissä";

    // Sulje sovellus kokonaan
    qApp->quit();

    // Käynnistä sovellus uudelleen
    QProcess::startDetached(QApplication::applicationFilePath());
}
