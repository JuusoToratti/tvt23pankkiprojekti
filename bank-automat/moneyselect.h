#ifndef MONEYSELECT_H
#define MONEYSELECT_H

#include <QWidget>

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
};

#endif // MONEYSELECT_H
