#include "open.h"
#include "ui_open.h"
#include <QPropertyAnimation>
open::open(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::open)
{

    //???
    QPropertyAnimation *animation = new QPropertyAnimation(this,"not now");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();


    ui->setupUi(this);
    this->setWindowTitle("WELCOME");                  //set a title to the window
    QIcon icon("./picture/aircon.png");               //icon
    this->setWindowIcon(icon);

    connect(ui->pushButton,SIGNAL(clicked()), *myAir,SLOT(openSignal()));

}

open::~open()
{
    delete ui;
}
