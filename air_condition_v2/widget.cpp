#include "widget.h"
#include "ui_widget.h"
#include<QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QDial>
#include <QDialog>
#include <QSpinBox>

Widget::Widget(QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent, flags),
    ui(new Ui::Widget)
{
     ui->setupUi(this);
     this->setWindowTitle("MENU");                  //set a title to the window
     QIcon icon("./picture/aircon.png");               //icon
     this->setWindowIcon(icon);


     //left(tab)
     m_list = new QListWidget;
     m_list->setFixedWidth(130);


    // m_list->setViewMode(QListWidget::IconMode);
     m_list->setIconSize(QSize(64,64));
     //m_list->setResizeMode(QListWidget::Adjust);
     m_list->addItem(new QListWidgetItem(QIcon("./picture/state.png"),"State"));
     m_list->addItem(new QListWidgetItem(QIcon("./picture/set.png"),"Set"));
     m_list->addItem(new QListWidgetItem(QIcon("./picture/more.png"),"More"));

     //right
     stack = new QStackedWidget(this);
     widgetmode = new QWidget;
     QWidget *widgetstate = new QWidget;

     stack->addWidget(widgetstate);
     stack->addWidget(widgetmode);

     QGridLayout *glayout2 = new QGridLayout;                    //state
     lbtmp = new QLabel(tr("temperature"));
     lbtmp2 = new QLabel(tr("25"));
     lbmode = new QLabel(tr("mode"));
     lbwindv = new QLabel(tr("wind"));
     lbkwh = new QLabel(tr("kwh"));
     lbbill = new QLabel(tr("bill"));

     glayout2->addWidget(lbtmp,0,0);
     glayout2->addWidget(lbtmp2,0,3);
     glayout2->addWidget(lbmode,1,0);
     glayout2->addWidget(lbwindv,2,0);
     glayout2->addWidget(lbkwh,3,0);
     glayout2->addWidget(lbbill,4,0);
     widgetstate->setLayout(glayout2);


     glayout = new QGridLayout;                   //setting
     lbtmp = new QLabel(tr("temperature"));
     lbmode = new QLabel(tr("mode"));
     lbwindv = new QLabel(tr("wind"));
     lbmode2 = new QLabel(tr("..."));
     lbwindv2 = new QLabel(tr("..."));

     qdial = new QDial;
     qdial->setMaximum(30);
     qdial->setMinimum(20);
     spbox = new QSpinBox;
     spbox->setMaximum(30);
     spbox->setMinimum(20);
     shut = new QPushButton;
     shut->setText("shut off");
     shut -> setStyleSheet("QPushButton{\
                           display: inline-block;\
                             padding: 6px 12px;\
                             margin-bottom: 0;\
                             font-size: 14px;\
                             font-weight: normal;\
                             line-height: 1.42857143;\
                             text-align: center;\
                             white-space: nowrap;\
                             vertical-align: middle;\
                             -ms-touch-action: manipulation;\
                                 touch-action: manipulation;\
                             -webkit-user-select: none;\
                                -moz-user-select: none;\
                                 -ms-user-select: none;\
                                     user-select: none;\
                             background-image: none;\
                             border: 1px solid transparent;\
                             border-radius: 4px;color:#fff;background-color:#5cb85c;border-color:#4cae4c\
                           }\
                           .focus {\
                             color: #fff;\
                             background-color: #449d44;\
                             border-color: #255625;\
                           }\
                           \
                           :hover{color: #fff;\
                             background-color: #449d44;\
                             border-color: #398439;\
                           cursor: pointer;\
                           }");

     connect(shut, SIGNAL(clicked()), this, SLOT(close()));
     glayout->addWidget(lbtmp,0,0);
     glayout->addWidget(qdial,0,1);
     glayout->addWidget(spbox,0,2);


     connect(qdial, SIGNAL(sliderMoved(int)), spbox, SLOT(setValue(int)));
     connect(spbox, SIGNAL(valueChanged(int)), qdial, SLOT(setValue(int)));

     glayout->addWidget(lbmode,1,0);
     glayout->addWidget(lbmode2,1,1);
     glayout->addWidget(lbwindv,2,0);
     glayout->addWidget(lbwindv2,2,1);
     glayout->addWidget(shut, 3,0);
     widgetmode->setLayout(glayout);


     moreinfo = new QLabel(tr("This system is create by Group.D, Class IX"));                    //more
     stack->addWidget(moreinfo);


     //design
     QHBoxLayout *mainLayout = new QHBoxLayout(this);
     mainLayout->addWidget(m_list);
     mainLayout->addWidget(stack, 0, Qt::AlignHCenter);
     mainLayout->setMargin(10);
     mainLayout->setSpacing(5);
     mainLayout->setStretchFactor(m_list,1);
     mainLayout->setStretchFactor(stack,1);
     connect(m_list,SIGNAL(currentRowChanged(int)),stack,SLOT(setCurrentIndex(int)));


     QDialog *dlg = new QDialog(this);
     dlg->setModal(true);
     dlg->show();

     /*
     QWidget *widgetbody = new QWidget(this);
     QWidget *widgetsidebar = new QWidget();
     QGridLayout *layoutbody =  new QGridLayout(this);

     QLabel *label1 = new QLabel;
     label1 -> setFixedWidth(100);

     widgetbody->QTool
     layoutbody->addWidget(widgetsidebar);
     //layoutbody->addItem(label1);
     widgetbody->setLayout(layoutbody);
     widgetbody->show();
     /*
     QWidget *widget = new QWidget(this);

     QListWidget* list_widget = new QListWidget;
     QHBoxLayout* layout = new QHBoxLayout;
     QLabel *label1 = new QLabel;
     label1 -> setFixedWidth(100);

     //here cannot input chinese, how?
     QListWidgetItem * lst1 = new QListWidgetItem("state", list_widget);
     QListWidgetItem* lst2 = new QListWidgetItem("settings", list_widget);
     QListWidgetItem* lst3 = new QListWidgetItem("...", list_widget);
     list_widget->insertItem(1, lst1);
     list_widget->insertItem(2, lst2);
     list_widget->insertItem(3, lst3);
     list_widget->show();
     layout->addWidget(list_widget);
     widget->setLayout(layout);
     QObject::connect(list_widget, SIGNAL(currentTextChanged(const QString &)), label1, SLOT(setText(const QString &)));
      widget->show();

*/


   /* ui->set->setText(tr("set"));
    ui->set->setIcon(QPixmap(":/picture/set.png"));
    ui->set->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);*/
}

Widget::~Widget()
{
    delete ui;
}



