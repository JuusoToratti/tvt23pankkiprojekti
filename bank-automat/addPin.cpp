#include "addPin.h"
#include "ui_addPin.h"
#include "mainuserinterface.h"
#include "ui_mainUserInterface.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QCloseEvent>
#include <QtSql>

addPin::addPin(QString cardNumber,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addPin)
    , cardNumber(cardNumber)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    //Liitä QPushButton::clicked-signaali handlePinInsert-slotiin
    connect(ui->pinSubmit,SIGNAL(clicked(bool)),
        this,SLOT(handlePinInsert()));

    // Yhdistä signaalit ja slotit
    connect(ui->n0, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n1, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n2, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n3, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n4, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n5, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n6, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n7, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n8, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n9, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->clear, &QPushButton::clicked, this, &addPin::clearLineEdit);

    // Luodaan timeri joka sulkee addPin.ui ja avaa MainWindow.ui 60 sekunnin jälkeen
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &addPin::timerTimeout);
    timer->start(60000);  
}

addPin::~addPin()
{
    delete ui;
}

void addPin::clearLineEdit()
{
    ui->pinLine->clear();
}

void addPin::timerTimeout()
{
    qDebug() << "Aika loppui";

    // Close the addPin window
    // this->close();

    // Create a new MainWindow and show it
    // MainWindow *mainWindow = new MainWindow();
    // mainWindow->show();

    // Sulje sovellus kokonaan
    qApp->quit();

    // Käynnistä sovellus uudelleen
    QProcess::startDetached(QApplication::applicationFilePath());
}

void addPin::closeEvent(QCloseEvent *event)
{
    // Pysäytetään timer kun addPin.ui sulkeutuu
    timer->stop();
    event->accept();
}

void addPin::numberClickedHandler()
{
    //Castaa klikatut numerot pinLine-tekstikenttään
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString number = button->text();
        ui->pinLine->setText(ui->pinLine->text() + number);
    }
}

void addPin::handlePinInsert()
{
    //jos rest-api puolella url muuttuu, niin tähän myös muutos
    QString card_url="http://localhost:3000/cardroutes/getcardnumberpin";
    QNetworkRequest requestPin((card_url));

    //WEBTOKEN ALKU
    //QByteArray myToken="Bearer "+webToken;
    //request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN LOPPU

    pgetManagerPin = new QNetworkAccessManager(this);
    connect(pgetManagerPin, SIGNAL(finished(QNetworkReply*)), this, SLOT(getPinSlot(QNetworkReply*)));
    preplyPin = pgetManagerPin->get(requestPin);

    qDebug() << "handlePinInsert funktiossa"; 
}

void addPin::getPinSlot(QNetworkReply *preplyPin)
{
    response_dataPin=preplyPin->readAll();
    qDebug()<<"DATA : "+response_dataPin;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_dataPin);
    QJsonObject json_obj = json_doc.object();
    correctPin=json_obj["pin"].toString();
    cardtype = json_obj["cardtype"].toInt();

    qDebug() << "Correct PIN: " << correctPin;
    qDebug() << "Card type: " << cardtype; // Tulosta myös kortin tyyppi

    qDebug() << "GetPinSlot funktiossa";

    // Luetaan syötetty arvo
    QString enteredPin = ui->pinLine->text();
    //short num = enteredPin.toShort();

    if (enteredPin == correctPin && cardtype == 0 && cardNumber == "-0600062093\r\n>")

    {
        qDebug() << "Oikea pin";

        //jos rest-api puolella url muuttuu, niin tähän myös muutos
        QString site_url="http://localhost:3000/users/user";
        QNetworkRequest request((site_url));

        //WEBTOKEN ALKU
        //QByteArray myToken="Bearer "+webToken;
        //request.setRawHeader(QByteArray("Authorization"),(myToken));
        //WEBTOKEN LOPPU

        pgetManager = new QNetworkAccessManager(this);
        connect(pgetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getNamesSlot(QNetworkReply*)));
        preply = pgetManager->get(request);

    } else {
        ui->insertPinLabel->setText("Väärä PIN-koodi");
    }
}

void addPin::getNamesSlot(QNetworkReply *preply)
{
    response_data=preply->readAll();
    //qDebug()<<"DATA : "+response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();

    QString cName;
    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
        if (json_obj["iduser"].toInt() == 2) { // Tarkistetaan iduserin arvo
            QString fname = json_obj["fname"].toString();
            QString lname = json_obj["lname"].toString();
            cName = "Hei " + fname + " " + lname + "!";
            break; // Keskeytetään silmukka, kun haluttu käyttäjä löytyy
            //cName+=QString::number(json_obj["iduser"].toInt())+", "+json_obj["fname"].toString()+", "+json_obj["lname"].toString()+"\r";
        }
    }

    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();

    QLabel *customerName = mainUserInterfaceWindow->findChild<QLabel*>("customerName");
    if (customerName) {
        customerName->setText(cName);
    }
    mainUserInterfaceWindow->show();

    //suljetaan nykyinen ikkuna
    this->close();

    preply->deleteLater();
    pgetManager->deleteLater();
}
