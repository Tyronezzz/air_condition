#include "json_handle.h"

json_handle::json_handle()
{
    this->myjson.insert(QString("type"),QString("temp"));
    this->myjson.insert(QString("temp"), int(22));
}

void json_handle::atry()
{
    QJsonDocument document;
    document.setObject(this->myjson);
    QByteArray byte_array=document.toJson(QJsonDocument::Compact);
    int size=byte_array.size();

    qDebug()<<byte_array;
    qDebug()<<"size is: "<<size;
}

