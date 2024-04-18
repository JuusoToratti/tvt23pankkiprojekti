#include "moneyselect.h"
#include "ui_moneyselect.h"
#include "mainuserinterface.h"
#include "ui_mainuserinterface.h"
#include "selectamount.h"
#include "ui_selectamount.h"

moneySelect::moneySelect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::moneySelect)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->backToMenu, &QPushButton::clicked, this, &moneySelect::handleBackToMenu);
    connect(ui->selectAmount, &QPushButton::clicked, this, &moneySelect::handleOtherAmount);
    connect(ui->twentyEuro, &QPushButton::clicked, this, &moneySelect::twentyEuroClickedSlot);
    connect(ui->fortyEuro, &QPushButton::clicked, this, &moneySelect::fortyEuroClickedSlot);
    connect(ui->fiftyEuro, &QPushButton::clicked, this, &moneySelect::fiftyEuroClickedSlot);
    connect(ui->hundredEuro, &QPushButton::clicked, this, &moneySelect::hundredEuroClickedSlot);
    //Tämä painike on olemassa testaustarkoituksiin
    connect(ui->insertMoney, &QPushButton::clicked, this, &moneySelect::insertHundredClickedSlot);
}

moneySelect::~moneySelect()
{
    delete ui;
}

void moneySelect::handleBackToMenu()
{
    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();
    mainUserInterfaceWindow->show();

    //suljetaan nykyinen ikkuna
    this->close();
}

void moneySelect::handleOtherAmount()
{
    // Luo uusi ikkuna ja käyttöliittymäolio
    selectAmount *selectAmountWindow = new selectAmount();
    selectAmountWindow->show();

    //suljetaan nykyinen ikkuna
    this->close();
}

void moneySelect::twentyEuroClickedSlot()
{
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",20);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*WEBTOKEN ALKU
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    WEBTOKEN LOPPU*/

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(update20Slot(QNetworkReply*)));

    reply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::update20Slot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<response_data;
    reply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // Tilin saldo menisi miinukselle, ei voida nostaa rahaa
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // Nosto onnistui ja tilin saldo pysyy positiivisena
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::fortyEuroClickedSlot()
{
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",40);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*WEBTOKEN ALKU
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    WEBTOKEN LOPPU*/

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(update40Slot(QNetworkReply*)));

    reply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::update40Slot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<response_data;
    reply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // Tilin saldo menisi miinukselle, ei voida nostaa rahaa
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // Nosto onnistui ja tilin saldo pysyy positiivisena
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::fiftyEuroClickedSlot()
{
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",50);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*WEBTOKEN ALKU
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    WEBTOKEN LOPPU*/

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(update50Slot(QNetworkReply*)));

    reply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::update50Slot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<"TEST1";
    qDebug()<<response_data;
    reply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // Tilin saldo menisi miinukselle, ei voida nostaa rahaa
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // Nosto onnistui ja tilin saldo pysyy positiivisena
        ui->chooseLabel->setText("Nosto onnistui");
    }
 }

void moneySelect::hundredEuroClickedSlot()
{
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",100);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*WEBTOKEN ALKU
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    WEBTOKEN LOPPU*/

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(update100Slot(QNetworkReply*)));

    reply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::update100Slot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<response_data;
    reply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // Tilin saldo menisi miinukselle, ei voida nostaa rahaa
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // Nosto onnistui ja tilin saldo pysyy positiivisena
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

//Alla olevat funktiot ovat testaustarkoituksiin

void moneySelect::insertHundredClickedSlot()
{
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",-100);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*WEBTOKEN ALKU
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    WEBTOKEN LOPPU*/

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateInsertedMoneySlot(QNetworkReply*)));

    reply = putManager->put(request, QJsonDocument(jsonObj).toJson());

    ui->chooseLabel->setText("100 € lisätty tilille");
}

void moneySelect::updateInsertedMoneySlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<response_data;
    reply->deleteLater();
    putManager->deleteLater();
}
