#include "widget.h"
#include <QApplication>
#include "open.h"
#include "login.h"
#include"jsonsocket.h"
#include"secondair.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f("Verdana",12);
    a.setFont(f);
    Widget w;
    open o1;
    login l1;

    if(o1.exec()==QDialog::Accepted)
    {
        l1.show();
        if(l1.exec()==QDialog::Accepted)
        {

            w.show();
            secondAir* myAir=new secondAir();
            return a.exec();
        }

        else
            return 0;
    }

    else
        return 0;

}





