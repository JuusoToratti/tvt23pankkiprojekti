#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

    // Tämä on vain esimerkki - muuta tätä vastaamaan todellista tietokantatoimintoa
    bool checkPIN(const QString &cardID, const QString &pin);

signals:

};

#endif // DATABASE_H

