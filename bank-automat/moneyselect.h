#ifndef MONEYSELECT_H
#define MONEYSELECT_H

#include <QWidget>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class moneySelect;
}
QT_END_NAMESPACE

class moneySelect : public QWidget
{
    Q_OBJECT

public:
    explicit moneySelect(QWidget *parent = nullptr);
    ~moneySelect();

private:
    Ui::moneySelect *ui;

private slots:
    void handleBackToMenu();
    void handleOtherAmount();
};

#endif // MONEYSELECT_H
