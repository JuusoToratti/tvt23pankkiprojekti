#include "selectamount.h"
#include "ui_selectamount.h"
#include "moneyselect.h"
#include "ui_moneyselect.h"

selectAmount::selectAmount(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::selectAmount)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Connect signals and slots
    connect(ui->N0, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N1, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N2, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N3, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N4, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N5, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N6, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N7, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N8, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N9, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->wipe, &QPushButton::clicked, this, &selectAmount::clearLe);
    connect(ui->backToMs,  &QPushButton::clicked, this, &selectAmount::backToMoneySelect);
    connect(ui->withdrawSa, &QPushButton::clicked, this, &selectAmount::checkAmount);
}

selectAmount::~selectAmount()
{
    delete ui;
}

void selectAmount::handleInsertedNum()
{

}

void selectAmount::numClickedHandler()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString number = button->text();
        ui->amountLe->setText(ui->amountLe->text() + number);
    }
}

void selectAmount::clearLe()
{
    ui->amountLe->clear();
}

void selectAmount::backToMoneySelect()
{
    close();

    // Avataan pääikkuna (mainwindow.ui)
    moneySelect *moneySelectWindow = new moneySelect();
    moneySelectWindow->show();
}

void selectAmount::checkAmount()
{
    // Luetaan syötetty arvo
    QString enteredNum = ui->amountLe->text();
    int n = enteredNum.toInt();

    if (n % 5 != 0) {
        //Nostettava määrä ei ole jaollinen viidellä
        ui->infoLabel->setText("Summaa ei voi nostaa");
    } else {
        //Tähän onnistunut muu nosto
        ui->infoLabel->setText("Summa nostettu");
    }
}
