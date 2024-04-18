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
#include <QStandardItem>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

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
    connect(ui->showTransactions, &QPushButton::clicked, this, &mainUserInterface::handleTransactionsClicked);
    connect(ui->showBalance, &QPushButton::clicked, this , &mainUserInterface::getCreditBalanceSlot);
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
    QString site_url="http://localhost:3000/accountinformation/info";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(transactionsNetworkReqFin(QNetworkReply*)));
    reply = getManager->get(request);
}

void mainUserInterface::transactionsNetworkReqFin(QNetworkReply *reply)
{
    //Tilitapahtumat

    QByteArray transResponse_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(transResponse_data);
    QJsonArray json_array = json_doc.array();
    QString accInfo;

    QStandardItemModel *table_model = new QStandardItemModel(json_array.size(),4);
    table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Tilinumero"));
    table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Noston numero"));
    table_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Määrä"));
    table_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Päiväys"));

    for (int row = 0; row < json_array.size(); ++row) {
        QJsonObject json_obj = json_array[row].toObject(); // Määritellään json_obj tässä
        QStandardItem *idaccount = new QStandardItem(QString::number(json_obj["idaccount"].toInt()));
        table_model->setItem(row, 0, idaccount);
        QStandardItem *transactions = new QStandardItem(json_obj["transactions"].toString());
        table_model->setItem(row, 1, transactions);
        QStandardItem *amount = new QStandardItem(json_obj["amount"].toString());
        table_model->setItem(row, 2, amount);
        QStandardItem *date = new QStandardItem(json_obj["date"].toString());
        table_model->setItem(row, 3, date);
    }

    qDebug()<<accInfo;

    // Etsi accwithdrawals-ikkuna ja aseta model sen transTableWidget:iin
    accWithdrawals *accWithdrawalsWindow = new accWithdrawals();
    accWithdrawalsWindow->transTableWidget(table_model);
    accWithdrawalsWindow->show();

    // Suljetaan mainUserInterface-ikkuna
    this->close();

    reply->deleteLater();
    getManager->deleteLater();
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
    //Debit balance alla

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

void mainUserInterface::logoutClicked()
{
    qDebug() << "logoutclickedissä";

    // Sulje sovellus kokonaan
    qApp->quit();

    // Käynnistä sovellus uudelleen
    QProcess::startDetached(QApplication::applicationFilePath());
}
