#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QLabel>
#include<QListWidget>
#include<QStackedWidget>
#include <QtGui>
#include<QGridLayout>
#include<QDial>
#include<QSpinBox>
#include<QPushButton>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
   // explicit Widget(QWidget *parent = 0);
    Widget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QLabel *lbtmp, *lbtmp2;
    QLabel *lbmode, *lbmode2;
    QLabel *lbwindv, *lbwindv2;
    QLabel *lbkwh, *lbkwh2;
    QLabel *lbbill, *lbbill2;
    QLabel *moreinfo;
    QListWidget *m_list;          //left sidebar
    QStackedWidget *stack;       //right
    QWidget *widgetmode;
    QGridLayout *glayout;
    QDial *qdial;
    QSpinBox *spbox;
    QPushButton *shut;
};

#endif // WIDGET_H
