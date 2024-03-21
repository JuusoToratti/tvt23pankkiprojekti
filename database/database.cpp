#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{

}

bool Database::checkPIN(const QString &cardID, const QString &pin)
{
    // Tässä toteutetaan todellinen tietokantatoiminto, joka tarkistaa tunnusluvun kortin ID:n perusteella
    // Palautetaan tässä esimerkissä aina true
    return true;
}

