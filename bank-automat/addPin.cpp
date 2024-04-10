#include "addPin.h"
#include "ui_addPin.h"
#include "mainuserinterface.h"
#include "ui_mainUserInterface.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QCloseEvent>

addPin::addPin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addPin)
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
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString number = button->text();
        ui->pinLine->setText(ui->pinLine->text() + number);
    }
}

void addPin::handlePinInsert()
{

    qDebug() << "handlePinInsert funktiossa";

    // Luetaan syötetty arvo
    QString enteredPin = ui->pinLine->text();
    short num = enteredPin.toShort();

    if (num == correctPin) {

        qDebug() << "Oikea pin";

        // Luo uusi ikkuna ja käyttöliittymäolio
        mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();
        mainUserInterfaceWindow->show();

        //suljetaan nykyinen ikkuna
        this->close();

    } else {
        ui->insertPinLabel->setText("Väärä PIN-koodi");
    }
}
