#include "addPin.h"
#include "ui_addPin.h"
#include "ui_mainUserInterface.h"
#include <QMessageBox>
#include <QDebug>

addPin::addPin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::addPin)
{
    ui->setupUi(this);

    //Liitä QPushButton::clicked-signaali handlePinInsert-slotiin
    connect(ui->pinSubmit, &QPushButton::clicked, this, &addPin::handlePinInsert);
}

addPin::~addPin()
{
    delete ui;
}

void addPin::handlePinInsert()
{
    // Luetaan syötetty arvo
    QString enteredPin = ui->pinLine->text();

    if (enteredPin == correctPin) {
        // Luo uusi käyttöliittymäolio
        Ui::MainUserInterface *secondUi = new Ui::MainUserInterface();

        // Aseta käyttöliittymä tähän ikkunaan
        secondUi->setupUi(this);

        // Piilota PIN-koodin lisäysikkuna
        this->close();
    } else {
        qDebug() << "Virheellinen PIN-koodi";
    }
}
