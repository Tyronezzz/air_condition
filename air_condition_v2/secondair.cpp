#include "secondair.h"

secondAir::secondAir()
{
    totallyConnect=false;
    freshTimer=0;
    windTimer=0;
    hasgetWind=false;
   // connect(this,SIGNAL(openSignal()),this,SLOT(InitAir()));
    connect(this,SIGNAL(openFalse()),this,SLOT(cantOpen()));
    connect(this,SIGNAL(closeFalse()),this,SLOT(cantClose()));
    connect(this,SIGNAL(closeSignal()),this,SLOT(shutdownAir()));
    this->isOpen=false;
    this->cur_Temp=25.00;
    this->roomId="101";
    this->UserId="001";
    this->aimIp="10.201.17.164";
    this->aimPort=8086;
    this->workMode="cold";
    mutexForSocket=new QMutex(QMutex::Recursive);
    mutexForCurTemp=new QMutex(QMutex::Recursive);
    openAir();

    upper_temp=30.00;
    lower_temp=20.00;
    lastTime_setTemp.start();

}

bool secondAir::openAir()
{

    if(!this->isOpen){
        emit openSignal();// 发出启动空调的信号

        return this->isOpen=true;
    }
    else{
        emit openFalse();
        return false;
    }

}

bool secondAir::closeAir()
{
    if(this->isOpen)
    {
        emit closeSignal();
        this->isOpen=false;
        return true;
    }
    else
    {
        emit closeFalse();
        return false;
    }
}

void secondAir::InitAir()
{
    //启动空调，建立socket连接
    mySocket=new jsonSocket(this,this->aimIp,this->aimPort);
    connect(mySocket,SIGNAL(getWind(int,QString)),this,SLOT(getWind(int,QString)));
    connect(mySocket,SIGNAL(setBill(double,double)),this,SLOT(setBill(double,double)));
    connect(mySocket,SIGNAL(setFresh(int)),this,SLOT(setFresh(int)));
    connect(mySocket,SIGNAL(setMode(QString,int)),this,SLOT(setMode(QString,int)));
    connect(this,SIGNAL(hereComeWind()),this,SLOT(workWind()));
    //启动socket接受信息线程
    readTimer=new QTimer(mySocket);
    readTimer->setInterval(10);
    connect(readTimer,SIGNAL(timeout()),this,SLOT(read()));
    readTimer->start();
//    readingThread=new readThread(mySocket,mutexForSocket);
//    readingThread->start();
//    connect(mySocket->socket,SIGNAL(disconnected()),readingThread,SLOT(quit()));
//    qDebug()<<"thread start";
    //设置温度传感器
    sensorTimer=new QTimer(mySocket);
    connect(sensorTimer,SIGNAL(timeout()),this,SLOT(sensorForTemp()));
    sensorTimer->setInterval(500);
    sensorTimer->start();
    //室内温度物理变化曲线
    tempNatureChange=new QTimer(mySocket);
    connect(tempNatureChange,SIGNAL(timeout()),this,SLOT(tempPhysics()));
    tempNatureChange->setInterval(60000);
    tempNatureChange->start();
    //启动空调后，先发一个temp包过去
    while(mySocket->socket->ConnectedState==0)
    {
        mySocket->socket->connectToHost(aimIp,aimPort);
    }
    mutexForCurTemp->lock();
    this->mySocket->sendTemp(cur_Temp);
    mutexForCurTemp->unlock();
    printTime();
    qDebug()<<"has sent temp,wait for fresh packet";
}

void secondAir::cantOpen()
{
    qDebug()<<"the air has already open";
}

void secondAir::shutdownAir()
{
    //空调关闭后各种释放
    readingThread->exit();
    readingThread->deleteLater();
    mySocket->deleteLater();
    freshTimer->deleteLater();
    totallyConnect=false;
    printTime();
    qDebug()<<"shut down success";
}

void secondAir::cantClose()
{
    qDebug()<<"the air has not been open";
}

void secondAir::setaimIpPort(QString IP, QString _port)
{
    this->aimPort=(quint16)_port.toInt();
    this->aimIp=IP;
}



void secondAir::setID(QString roomID, QString User)
{
    this->roomId=roomID;
    this->UserId=User;
}

void secondAir::inTimeSendTemp()
{
    mutexForSocket->lock();
    mutexForCurTemp->lock();
    mySocket->sendTemp(this->cur_Temp);
    mutexForCurTemp->unlock();
    mutexForSocket->unlock();
}

void secondAir::getWind(int _windTemp, QString velocity)
{
    if(!hasgetWind){
        hasgetWind=true;
        windTemp=_windTemp;
        windType=velocity;
        emit hereComeWind();
    }
}

void secondAir::setBill(double kwh, double bill)
{
    qDebug()<<"get bill";
    qDebug()<<"kwh: "<<kwh;
    qDebug()<<"bill: "<<bill;
    this->powerNum=kwh;
    this->cost=bill;
}

void secondAir::setFresh(int freshrate)
{
    qDebug()<<"get fresh packet";
    qDebug()<<"rate: "<<freshrate;
    this->freshSecond=freshrate;
    if(!totallyConnect){//判断是否已经和主控机建立好连接
        //先回一个auth
        this->mySocket->sendAuth(this->roomId,this->UserId);
        printTime();
        qDebug()<<"auth packet has sent,wait for mode packet";
    }
    printTime();
    //设置实时温度返回包
    if(freshTimer!=0)
        delete freshTimer;
    freshTimer=new QTimer(this->mySocket);
    freshTimer->setInterval(freshrate*1000);
    connect(freshTimer,SIGNAL(timeout()),this,SLOT(sendCurTemp()));
    freshTimer->start();
}

void secondAir::setMode(QString mode, int temp)
{
    qDebug()<<"get mode";
    qDebug()<<"mode: "<<mode;
    qDebug()<<"temp: "<<temp;
    aim_Temp=temp;
    this->workMode=mode;
    printTime();
    if(!totallyConnect){
        totallyConnect=true;
        qDebug()<<"get the mode packet,now start working!";
    }
    lastTime_setTemp.restart();
}

void secondAir::setTemp(double temp)
{
    if(lastTime_setTemp.elapsed()<=1000){
        qDebug()<<"set temp too fast,plz wait for a second";
        return ;
    }
    else{
        lastTime_setTemp.restart();
    }

    if(temp<=upper_temp&&temp>=lower_temp){
        qDebug()<<"set success";
        aim_Temp=temp;

    }
    else{
        qDebug()<<"beyong the limit!";
    }
}

void secondAir::sendCurTemp()
{
    mutexForSocket->lock();
    mutexForCurTemp->lock();
    mySocket->sendTemp(this->cur_Temp);
    mutexForCurTemp->unlock();
    mutexForSocket->unlock();
}

void secondAir::sensorForTemp()
{
    printTime();
    mutexForCurTemp->lock();
    qDebug()<<cur_Temp;
    mutexForCurTemp->unlock();
    if(aim_Temp-cur_Temp>0.05||cur_Temp-aim_Temp>0.05){
        if(!hasgetWind){
           // askWind();
        }
    }
}

void secondAir::tempPhysics()
{
    mutexForCurTemp->lock();
    if(workMode=="COLD"){//制冷模式下说明是夏天，在0-每分钟自动上升0.5
        if(cur_Temp<27.00){
            cur_Temp+=0.5;
        }
        else{
            cur_Temp+=0.3;
        }
    }
    else{//暖气为冬天，每分钟下降0.5
        if(cur_Temp<10.00){
            cur_Temp-=0.3;
        }
        else{
            cur_Temp-=0.5;
        }
    }
    printTime();
    //qDebug()<<"temp change to :"<<cur_Temp;
    mutexForCurTemp->unlock();
}

void secondAir::read()
{
    mutexForSocket->lock();
    if(mySocket->socket->bytesAvailable()>0){
        QByteArray byte_array =mySocket->socket->readLine();
        mySocket->myjsonParse(byte_array);
        //qDebug()<<byte_array;
    }
    mutexForSocket->unlock();
}

void secondAir::askWind()
{
    double level=qAbs(this->cur_Temp-this->aim_Temp);
    if(level>5.00){
        mutexForSocket->lock();
        this->mySocket->sendNeedWind((int)aim_Temp,"HIGH");
        mutexForSocket->unlock();
    }
    else if(level>2.00){
        mutexForSocket->lock();
        this->mySocket->sendNeedWind((int)aim_Temp,"MEDIUM");
        mutexForSocket->unlock();
    }
    else if(level>0.05){
        mutexForSocket->lock();
        this->mySocket->sendNeedWind((int)aim_Temp,"LOW");
        mutexForSocket->unlock();
    }
    else {
        mutexForSocket->lock();
        this->mySocket->sendNeedWind((int)aim_Temp,"NONE");
        mutexForSocket->unlock();
    }
    qDebug()<<"send the wind packet";
}

void secondAir::workWind()
{
    //启动风工作
    if(windTimer!=0){
        delete windTimer;
    }
    windTimer=new QTimer(mySocket);
    windTimer->setInterval(60000);
    connect(windTimer,SIGNAL(timeout()),this,SLOT(windWorking()));
    windTimer->start();

}

void secondAir::windWorking()
{

}





void secondAir::sendLinkRequest()
{
    //{"type"="auth", "room"="A15", "ID"="123456789012344567"}]
    mutexForSocket->lock();
    mySocket->sendInfo(this->roomId,this->UserId);
    mutexForSocket->unlock();
}

void secondAir::printTime()
{
    //qDebug()<<"now time is: "<<QTime::currentTime().toString();
}
