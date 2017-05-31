#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonValue>

//Purpose: get freshrate from the host
void getfreshrate(QByteArray byteArray)
{
    QJsonParseError jsonError;

    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
    if(!document.isNull() && (jsonError.error==QJsonParseError::NoError))
    {
        QJsonObject object = document.object();
        if(object.contains("freshperiod"))
        {
            QJsonValue value = object.value("freshperiod");     //get the value of freshrate
            if(value.isDouble())
            {
                //= value.toDouble();             //

            }

        }





    }

}
