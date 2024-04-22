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

    connect(ui->twentyEuro, &QPushButton::clicked, this, &moneySelect::twentyEuroClickedPut);
    connect(ui->twentyEuro, &QPushButton::clicked, this, &moneySelect::twentyEuroClickedGet);

    connect(ui->fortyEuro, &QPushButton::clicked, this, &moneySelect::fortyEuroClickedPut);
    connect(ui->fortyEuro, &QPushButton::clicked, this, &moneySelect::fortyEuroClickedGet);

    connect(ui->fiftyEuro, &QPushButton::clicked, this, &moneySelect::fiftyEuroClickedPut);
    connect(ui->fiftyEuro, &QPushButton::clicked, this, &moneySelect::fiftyEuroClickedGet);

    connect(ui->hundredEuro, &QPushButton::clicked, this, &moneySelect::hundredEuroClickedPut);
    connect(ui->hundredEuro, &QPushButton::clicked, this, &moneySelect::hundredEuroClickedGet);
    //Tämä painike on olemassa testaustarkoituksiin
    connect(ui->insertMoney, &QPushButton::clicked, this, &moneySelect::insertHundredClickedPut);
    connect(ui->insertMoney, &QPushButton::clicked, this, &moneySelect::insertHundredClickedPost);
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

void moneySelect::twentyEuroClickedPut()
{
     qDebug()<<"twentyEuroClickedPut-funktiossa";

    // PUT-metodi

    QJsonObject putObj;
    putObj.insert("account_balance",20);

    QString put_url="http://localhost:3000/accounts/update";
    QNetworkRequest putRequest((put_url));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*WEBTOKEN ALKU
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    WEBTOKEN LOPPU*/

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put20Slot(QNetworkReply*)));

    QJsonDocument putDoc(putObj);
    QByteArray putData = putDoc.toJson();

    putReply = putManager->put(putRequest, putData);
}

void moneySelect::put20Slot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
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

void moneySelect::twentyEuroClickedGet()
{
    // Suorita GET-pyyntö tilin saldon hakemiseksi
    QString site_url = "http://localhost:3000/accounts/getaccountbalance";
    QNetworkRequest getRequest(site_url);

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(twentyEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::twentyEuroClickedPost(QNetworkReply *getReply)
{
    // Varmista, että vastaus on saatavilla ja ei ole tapahtunut virhettä
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Käsittele vastaus JSON-muodossa
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonArray json_array = json_doc.array();

        QString cCredit;

        foreach (const QJsonValue &value, json_array) {
            QJsonObject json_obj = value.toObject();
            QString account_balance = json_obj["account_balance"].toString();
            cCredit = account_balance;
            qDebug() << "Account balance: " << cCredit;
        }

        // Tarkista tilin saldo ja tee POST-pyyntö vain, jos saldo riittää
        if (cCredit.toDouble() >= 20.0) {
            // Jos tilin saldo riittää, tee POST-pyyntö
            QJsonObject postObj;
            postObj.insert("idaccount", 1); //tähän oikea arvo
            postObj.insert("transactions", "1"); //tähän oikea arvo
            postObj.insert("amount", -20);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Lisää nykyinen päivämäärä

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post20Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
            // Tähän voit lisätä tarvittavan logiikan, esim. käyttäjälle näytettävän virheilmoituksen
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
        // Tähän voit lisätä tarvittavan virheen käsittelyn
    }

    // Vapauta resurssit
    getReply->deleteLater();
    getManager->deleteLater();
}

void moneySelect::post20Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::fortyEuroClickedPut()
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
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put40Slot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::put40Slot(QNetworkReply *PutReply)
{
    putResponse_data=PutReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
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

void moneySelect::fortyEuroClickedGet()
{
    // Suorita GET-pyyntö tilin saldon hakemiseksi
    QString site_url = "http://localhost:3000/accounts/getaccountbalance";
    QNetworkRequest getRequest(site_url);

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fortyEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::fortyEuroClickedPost(QNetworkReply *getReply)
{
    // Varmista, että vastaus on saatavilla ja ei ole tapahtunut virhettä
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Käsittele vastaus JSON-muodossa
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonArray json_array = json_doc.array();

        QString cCredit;

        foreach (const QJsonValue &value, json_array) {
            QJsonObject json_obj = value.toObject();
            QString account_balance = json_obj["account_balance"].toString();
            cCredit = account_balance;
            qDebug() << "Account balance: " << cCredit;
        }

        // Tarkista tilin saldo ja tee POST-pyyntö vain, jos saldo riittää
        if (cCredit.toDouble() >= 40.0) {
            // Jos tilin saldo riittää, tee POST-pyyntö
            QJsonObject postObj;
            postObj.insert("idaccount", 1); //tähän oikea arvo
            postObj.insert("transactions", "1"); //tähän oikea arvo
            postObj.insert("amount", -40);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Lisää nykyinen päivämäärä

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post40Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
            // Tähän voit lisätä tarvittavan logiikan, esim. käyttäjälle näytettävän virheilmoituksen
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
        // Tähän voit lisätä tarvittavan virheen käsittelyn
    }

    // Vapauta resurssit
    getReply->deleteLater();
    getManager->deleteLater();
}

void moneySelect::post40Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::fiftyEuroClickedPut()
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
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put50Slot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::put50Slot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
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

void moneySelect::fiftyEuroClickedGet()
{
    // Suorita GET-pyyntö tilin saldon hakemiseksi
    QString site_url = "http://localhost:3000/accounts/getaccountbalance";
    QNetworkRequest getRequest(site_url);

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fiftyEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::fiftyEuroClickedPost(QNetworkReply *getReply)
{
    // Varmista, että vastaus on saatavilla ja ei ole tapahtunut virhettä
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Käsittele vastaus JSON-muodossa
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonArray json_array = json_doc.array();

        QString cCredit;

        foreach (const QJsonValue &value, json_array) {
            QJsonObject json_obj = value.toObject();
            QString account_balance = json_obj["account_balance"].toString();
            cCredit = account_balance;
            qDebug() << "Account balance: " << cCredit;
        }

        // Tarkista tilin saldo ja tee POST-pyyntö vain, jos saldo riittää
        if (cCredit.toDouble() >= 50.0) {
            // Jos tilin saldo riittää, tee POST-pyyntö
            QJsonObject postObj;
            postObj.insert("idaccount", 1); //tähän oikea arvo
            postObj.insert("transactions", "1"); //tähän oikea arvo
            postObj.insert("amount", -50);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Lisää nykyinen päivämäärä

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post50Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
            // Tähän voit lisätä tarvittavan logiikan, esim. käyttäjälle näytettävän virheilmoituksen
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
        // Tähän voit lisätä tarvittavan virheen käsittelyn
    }

    // Vapauta resurssit
    getReply->deleteLater();
    getManager->deleteLater();
}

void moneySelect::post50Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::hundredEuroClickedPut()
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
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put100Slot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::put100Slot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
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

void moneySelect::hundredEuroClickedGet()
{
    // Suorita GET-pyyntö tilin saldon hakemiseksi
    QString site_url = "http://localhost:3000/accounts/getaccountbalance";
    QNetworkRequest getRequest(site_url);

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(hundredEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::hundredEuroClickedPost(QNetworkReply *getReply)
{
    // Varmista, että vastaus on saatavilla ja ei ole tapahtunut virhettä
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Käsittele vastaus JSON-muodossa
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonArray json_array = json_doc.array();

        QString cCredit;

        foreach (const QJsonValue &value, json_array) {
            QJsonObject json_obj = value.toObject();
            QString account_balance = json_obj["account_balance"].toString();
            cCredit = account_balance;
            qDebug() << "Account balance: " << cCredit;
        }

        // Tarkista tilin saldo ja tee POST-pyyntö vain, jos saldo riittää
        if (cCredit.toDouble() >= 100.0) {
            // Jos tilin saldo riittää, tee POST-pyyntö
            QJsonObject postObj;
            postObj.insert("idaccount", 1); //tähän oikea arvo
            postObj.insert("transactions", "1"); //tähän oikea arvo
            postObj.insert("amount", -100);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Lisää nykyinen päivämäärä

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post100Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
            // Tähän voit lisätä tarvittavan logiikan, esim. käyttäjälle näytettävän virheilmoituksen
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
        // Tähän voit lisätä tarvittavan virheen käsittelyn
    }

    // Vapauta resurssit
    getReply->deleteLater();
    getManager->deleteLater();
}

void moneySelect::post100Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

//Alla olevat funktiot ovat testaustarkoituksiin

void moneySelect::insertHundredClickedPut()
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
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(putInsertedMoneySlot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());

    ui->chooseLabel->setText("100 € lisätty tilille");
}

void moneySelect::putInsertedMoneySlot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();
}

void moneySelect::insertHundredClickedPost()
{
    QJsonObject postObj;
    postObj.insert("idaccount", 1); //tähän oikea arvo
    postObj.insert("transactions", "1"); //tähän oikea arvo
    postObj.insert("amount", +100);
    postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Lisää nykyinen päivämäärä

    QString post_url = "http://localhost:3000/accountinformation/create";
    QNetworkRequest postRequest(post_url);
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    postManager = new QNetworkAccessManager(this);
    connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postInsertedMoneySlot(QNetworkReply*)));

    QJsonDocument postDoc(postObj);
    QByteArray postData = postDoc.toJson();

    postReply = postManager->post(postRequest, postData);
}

void moneySelect::postInsertedMoneySlot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}
