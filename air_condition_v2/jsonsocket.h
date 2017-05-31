#ifndef JSONSOCKET_H
#define JSONSOCKET_H

#include<QTcpSocket>
#include<QJsonObject>
#include<QArrayData>
#include<QJsonDocument>
#include<QHostAddress>
#include<QTimer>
#include<QJsonArray>
class jsonSocket:public QObject
{
    Q_OBJECT
public:
    jsonSocket(QObject *parent=0,QString IP="127.0.0.1",quint16 port=8086);
    QTcpSocket* socket;
    QTimer *mytimer1,*mytimer2;
    void sendTemp(int temperature);//向主机发送温度测量
    void sendInfo(QString room,QString ID);//向主机发送验证信息
    void sendNeedWind(int desttemp,QString velocity);//送风请求
    void sendNoNeedWind();//停止送风请求
    void sendAuth(QString room,QString ID);
    void myjsonParse(QByteArray data);
    void myParseObject(QJsonObject obj);
    void dealMode(QJsonObject jsonObj);
    void dealFresh(QJsonObject jsonObj);
    void dealWind(QJsonObject jsonObj);
    void dealBill(QJsonObject jsonObj);
    void haruharu();
signals:
    void isConnected();
    void getWind(int,QString);
    void setBill(double,double);
    void setFresh(int);
    void setMode(QString,int);
private slots:
    void haveConnect();
    void timeSender1();
    void timeSender2();
    void stopConnect();
    void showError(QAbstractSocket::SocketError socketError);
public slots:

};

#endif // JSONSOCKET_H
