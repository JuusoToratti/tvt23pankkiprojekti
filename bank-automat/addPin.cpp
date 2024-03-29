#include "addPin.h"
#include "ui_addPin.h"
#include "mainuserinterface.h"
#include "ui_mainUserInterface.h"
#include <QMessageBox>
#include <QDebug>

addPin::addPin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addPin)
{
    ui->setupUi(this);

    //Liitä QPushButton::clicked-signaali handlePinInsert-slotiin
    connect(ui->pinSubmit,SIGNAL(clicked(bool)),
        this,SLOT(handlePinInsert()));

    // Connect signals and slots
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
}

addPin::~addPin()
{
    delete ui;
}

void addPin::clearLineEdit()
{
    ui->pinLine->clear();
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
        qDebug() << "Virheellinen PIN-koodi";
    }
}
