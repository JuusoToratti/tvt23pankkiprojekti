#include "cdchoice.h"
#include "ui_cdchoice.h"
#include "mainuserinterface.h"
#include <QTimer>
#include <QProcess>
#include <QCloseEvent>

cdChoice::cdChoice(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cdChoice)
{
    ui->setupUi(this);

    connect(ui->creditButton, &QPushButton::clicked, this, &cdChoice::creditClicked);
    connect(ui->debitButton, &QPushButton::clicked, this, &cdChoice::debitClicked);

    // Create a timer to close the cdChoice window and open the MainWindow after 60 seconds
    timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &cdChoice::timerOut);
    timer2->start(60000);
}

cdChoice::~cdChoice()
{
    delete ui;
}

void cdChoice::creditClicked()
{
    this->close();

    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();
    mainUserInterfaceWindow->show();
}

void cdChoice::debitClicked()
{
    this->close();

    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();
    mainUserInterfaceWindow->show();
}

void cdChoice::timerOut()
{
    // Sulje sovellus kokonaan
    qApp->quit();

    // Käynnistä sovellus uudelleen
    QProcess::startDetached(QApplication::applicationFilePath());
}

void cdChoice::closeEvent(QCloseEvent *event)
{
    // Stop the timer when cdChoice window is closed
    timer2->stop();
    event->accept();
}
