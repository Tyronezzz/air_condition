#include "readthread.h"

void readThread::run()
{
    while(1)
    {
        mutex->lock();
        if(readSocket->socket->bytesAvailable()>0){
            QByteArray byte_array =readSocket->socket->readLine();
            readSocket->myjsonParse(byte_array);
            //qDebug()<<byte_array;
        }
        mutex->unlock();

    }
}

readThread::readThread(jsonSocket* theSocket,QMutex* _mutex)
{
    readSocket=theSocket;
    mutex=_mutex;
    connect(readSocket->socket,SIGNAL(disconnected()),this,SLOT(readThread::quit()));
}
