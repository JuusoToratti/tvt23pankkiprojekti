#ifndef CDCHOICE_H
#define CDCHOICE_H

#include <QWidget>

namespace Ui {
class cdChoice;
}

class cdChoice : public QWidget
{
    Q_OBJECT

public:
    explicit cdChoice(QWidget *parent = nullptr);
    ~cdChoice();

private:
    Ui::cdChoice *ui;
    QTimer *timer2;

private slots:
    void creditClicked();
    void debitClicked();
    void timerOut();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CDCHOICE_H
