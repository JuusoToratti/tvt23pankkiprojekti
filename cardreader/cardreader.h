#ifndef CARDREADER_H
#define CARDREADER_H

#include <QObject>
#include <QTimer> // Include QTimer header

class CardReader : public QObject
{
    Q_OBJECT
public:
    explicit CardReader(QObject *parent = nullptr);

signals:
    void cardInserted(const QString &cardID);

private slots:
    void simulateCardInsertion();

private:
    QTimer *timer; // Corrected to QTimer
};

#endif // CARDREADER_H
