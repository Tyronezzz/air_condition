#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowTitle("LOGIN");                  //set a title to the window
    QIcon icon("./picture/aircon.png");               //icon
    this->setWindowIcon(icon);

    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    connect(ui->loginz, SIGNAL(clicked()), this ,SLOT(InitAir()));

}

login::~login()
{
    delete ui;
}
