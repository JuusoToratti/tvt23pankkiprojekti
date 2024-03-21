#include "cardreader.h"

CardReader::CardReader(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this); // Corrected to QTimer
    connect(timer, &QTimer::timeout, this, &CardReader::simulateCardInsertion);
    timer->start(5000); // Simuloi kortinlukijan tarkistusta 5 sekunnin välein
}

void CardReader::simulateCardInsertion()
{
    // Simuloi kortinlukijan löytävän kortin ja lähettävän signaalin kortin ID:stä
    QString cardID = "123456789"; // Tässä voit asettaa haluamasi kortin ID:n
    emit cardInserted(cardID);
}
