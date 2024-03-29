#include "moneyselect.h"
#include "ui_moneyselect.h"
#include "mainuserinterface.h"
#include "ui_mainuserinterface.h"

moneySelect::moneySelect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::moneySelect)
{
    ui->setupUi(this);

    connect(ui->backToMenu, &QPushButton::clicked, this, &moneySelect::handleBackToMenu);
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
