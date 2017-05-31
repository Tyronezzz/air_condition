#ifndef OPEN_H
#define OPEN_H

#include <QDialog>

namespace Ui {
class open;
}

class open : public QDialog
{
    Q_OBJECT

public:
    explicit open(QWidget *parent = 0);
    ~open();

private:
    Ui::open *ui;
};

#endif // OPEN_H
