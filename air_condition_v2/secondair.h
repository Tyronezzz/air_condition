#ifndef SECONDAIR_H
#define SECONDAIR_H
#include<QString>
#include<QJsonObject>
#include"jsonsocket.h"
#include<QTimer>
#include<QMutex>
#include"readthread.h"
#include<QTime>
class secondAir:public QObject
{
    Q_OBJECT
public:
    secondAir();
    //工作模式 缺省工作温度 真实工作温度  开关状态
signals:
    void openFalse();//空调关闭失败信号
    void openSignal();//空调启动成功信号
    void closeSignal();//空调关闭成功信号
    void closeFalse();//空调关闭失败信号
    void hereComeWind();//收到来自主控机风的信号，由这个信号来出发房间温度根据风来变化
private slots:
    bool openAir();//开启空调
    bool closeAir();//关闭空调
    void InitAir();//当空调开启时候触发
    void cantOpen();//开启失败触发
    void shutdownAir();//关闭触发
    void cantClose();//关闭失败触发
    void setaimIpPort(QString IP,QString port);
    void setID(QString roomID,QString User);
    void inTimeSendTemp();
    void getWind(int,QString);
    void setBill(double,double);
    void setFresh(int);//收到 freshrate包，设置向主控机发送更新包的频率并启动计时器
    void setMode(QString,int);//收到mode包设置房间的工作模式和缺省温度
    void setTemp(double);//设置房间的目标温度
    void sendCurTemp();//向主控机发送实时温度
    void sensorForTemp();//房间传感器实时感应温度
    void tempPhysics();//房间的物理自然变化
    void read();//读来自主控机的请求
    void askWind();//向服务器请求风
    void workWind();//初始化风的计时器
    void windWorking();//风作用于房间
private:
    void sendLinkRequest();//发送连接请求
    jsonSocket *mySocket;//负责发送请求
    QTimer* freshTimer;//负责定时发送温度
    QTimer* sensorTimer;//温度传感器
    QTimer* tempNatureChange;//房间物理变化的计时器
    QTimer* readTimer;//socket读的计时器
    QTimer* windTimer;//根据收到的风变化房间温度的计时器
    QMutex* mutexForSocket;//锁用于 socket的读写
    QMutex* mutexForCurTemp;//锁用于 房间当前温度的读写
    readThread* readingThread;//弃用
    double upper_temp,lower_temp;//房间的温度上限和下限
    QTime lastTime_setTemp;//上一次修改目标温度的时间，用来判断是否1s内发出多次请求
    int freshSecond;//多久更新温度一次
    void printTime();//打印时间的调试函数
    bool totallyConnect;//和主控机完全建立了连接，已收到mode包为标准
    bool hasgetWind;//已经受到来自主控机的风
    int windTemp;//收到的风的温度
    QString windType;//收到的风的大小
    QString workMode;//工作模式
    double cur_Temp,aim_Temp;//当前温度和目标温度
    QString roomId;//房间ID
    QString UserId;//顾客身份
    bool isOpen;//是否开启
    //physic phusic_module;//物理模块，定义数学曲线，房间温度变化 实现aim_Temp动态变化（开个线程）
    //Socket socket;//通信模块 负责 发送请求，处理请求
    //gui 用户界面模块 显示和按钮
    //WindType windType;//风类型
    double powerNum;//用量
    double cost;//金额
    QString aimIp;//主机IP
    quint16 aimPort;//主机端口


};

#endif // SECONDAIR_H
