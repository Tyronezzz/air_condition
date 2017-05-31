#include "jsonsocket.h"

jsonSocket::jsonSocket(QObject *parent,QString IP,quint16 port)
{
    socket=new QTcpSocket(parent);

    socket->connectToHost(IP,port);
    connect(socket,SIGNAL(connected()),this,SLOT(haveConnect()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(stopConnect()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(showError(QAbstractSocket::SocketError)));
    qDebug()<<"start trying connect";
}

void jsonSocket::haveConnect()
{

    qDebug()<<"connect success!";
}

void jsonSocket::timeSender2()
{
    //[{"type"="startwind", "desttemp"=22, "velocity"="HIGH"}]
    QJsonObject myjson;
    myjson.insert("type","startwind");
    myjson.insert("destemp",22);
    myjson.insert("velocity","HIGH");
    QJsonDocument document;
    document.setObject(myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    socket->write(byte_array);
}

void jsonSocket::timeSender1()
{
    QJsonObject myjson;
    myjson.insert("type","bill");
    myjson.insert("kwh",2.55);
    myjson.insert("bill",12.11);
    QJsonDocument document;
    document.setObject(myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    socket->write(byte_array);
}

void jsonSocket::stopConnect()
{
    qDebug()<<"stop connect";

    socket->deleteLater();
}

void jsonSocket::showError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"error is: "<<socketError;
}

void jsonSocket::sendTemp(int temperature)
{
    QJsonObject myjson;
    myjson.insert("type","temp");
    myjson.insert("temp",temperature);
    QJsonDocument document;
    document.setObject(myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    socket->write(byte_array);
}

void jsonSocket::sendInfo(QString room, QString ID)
{
    QJsonObject myjson;
    myjson.insert("type","auth");
    myjson.insert("room",room);
    myjson.insert("ID",ID);
    QJsonDocument document;
    document.setObject(myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    socket->write(byte_array);
}

void jsonSocket::sendNeedWind(int desttemp, QString velocity)
{
    QJsonObject myjson;
    myjson.insert("type","startwind");
    myjson.insert("desttemp",desttemp);
    myjson.insert("velocity",velocity);
    QJsonDocument document;
    document.setObject(myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    socket->write(byte_array);
}

void jsonSocket::sendNoNeedWind()
{
    QJsonObject myjson;
    myjson.insert("type","stopwind");
    QJsonDocument document;
    document.setObject(myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    socket->write(byte_array);
}

void jsonSocket::sendAuth(QString room, QString ID)
{
    QJsonObject myjson;
    myjson.insert("type","auth");
    myjson.insert("room",room);
    myjson.insert("ID",ID);
    QJsonDocument document;
    document.setObject(myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    socket->write(byte_array);
}

void jsonSocket::myjsonParse(QByteArray data)
{
    QJsonParseError jsonError;
    QJsonDocument parseDoc = QJsonDocument::fromJson(data,&jsonError);
    QString str(data);
    //[{"type"="bill", "kwh"=0.01, "bill"=0.01}]
    //[{"type"="wind", "windtemp"=20, "velocity"="HIGH"}]
    //[{"type"="mode", "workingmode"="COLD", "defaulttemp"=20}]
    //[{"type"="freshrate", "freshperiod"=3}]
    if(jsonError.error==QJsonParseError::NoError){
        if(parseDoc.isObject())
        {
            myParseObject(parseDoc.object());
        }
    }


}

void jsonSocket::myParseObject(QJsonObject obj)
{
    //qDebug()<<obj.count();
    if(obj.contains("kwh")){
        dealBill(obj);
    }
    if(obj.contains("freshperiod")){
        dealFresh(obj);
    }
    if(obj.contains("workingmode")){
        dealMode(obj);
    }
    if(obj.contains("windtemp")){
        dealWind(obj);
    }
}

void jsonSocket::dealMode(QJsonObject jsonObj)//设置工作模式
{
    //qDebug()<<"type :mode";
    QString workingmode;
    int defaulttemp;
    if(jsonObj.contains("workingmode")){
        QJsonValue workingmodeValue=jsonObj.take("workingmode");
        if(workingmodeValue.isString()){
            workingmode=workingmodeValue.toString();
            //qDebug()<<"workingmode :"<<workingmode;
        }
    }
    if(jsonObj.contains("defaulttemp")){
        QJsonValue defaulttempValue=jsonObj.take("defaulttemp");
        if(defaulttempValue.isDouble()){
            defaulttemp=defaulttempValue.toInt();
            //qDebug()<<"defaulttemp :"<<defaulttemp;
        }
    }
    emit setMode(workingmode,defaulttemp);
}

void jsonSocket::dealFresh(QJsonObject jsonObj)//设置更新频率
{
    //()<<"type :freshrate";
    if(jsonObj.contains("freshperiod")){
        QJsonValue freshperiodValue=jsonObj.take("freshperiod");
        if(freshperiodValue.isDouble()){
            int data=freshperiodValue.toInt();
           // qDebug()<<"freshperiod :"<<data;
            emit setFresh(data);
        }
    }
}

void jsonSocket::dealWind(QJsonObject jsonObj)//接受风
{
   // qDebug()<<"type :wind";
    int data;
    QString velocity;
    if(jsonObj.contains("windtemp")){
        QJsonValue windTempValue=jsonObj.take("windtemp");
        if(windTempValue.isDouble()){
            data=windTempValue.toInt();
           // qDebug()<<"windtemp :"<<data;
        }
    }
    if(jsonObj.contains("velocity")){
        QJsonValue velocityValue=jsonObj.take("velocity");
        if(velocityValue.isString()){
            velocity=velocityValue.toString();
          //  qDebug()<<"velocity :"<<velocity;
        }
    }
    emit getWind(data,velocity);
}

void jsonSocket::dealBill(QJsonObject jsonObj)
{
   // qDebug()<<"type :bill";
    double kwh,bill;
    if(jsonObj.contains("kwh")){
        QJsonValue kwhValue=jsonObj.take("kwh");
        if(kwhValue.isDouble()){
            kwh=kwhValue.toDouble();
            //qDebug()<<"kwh :"<<kwh;
        }
        else{
            //qDebug()<<"kwh is not double!";
        }
    }
    if(jsonObj.contains("bill")){
        QJsonValue billValue=jsonObj.take("bill");
        if(billValue.isDouble()){
            bill=billValue.toDouble();
           // qDebug()<<"bill :"<<bill;
        }
        else{
           // qDebug()<<"bill is not double!";
        }
    }
    emit setBill(kwh,bill);
}

void jsonSocket::haruharu()
{

}


