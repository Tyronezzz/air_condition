#ifndef READTHREAD_H
#define READTHREAD_H
#include<QThread>
#include<QMutex>
#include"jsonsocket.h"
class readThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE ;
public:
    readThread(jsonSocket* theSocket,QMutex* _mutex);
    jsonSocket *readSocket;
    QMutex* mutex;
signals:
};

#endif // READTHREAD_H


