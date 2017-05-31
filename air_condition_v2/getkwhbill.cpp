#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonValue>

//Purpose: get the kwh and the bill from host
void getkwhbill(QByteArray byteArray)
{
    QJsonParseError jsonError;

    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
    if(!document.isNull() && (jsonError.error==QJsonParseError::NoError) )
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();     //convert jsondoc into object
            if(object.contains("kwh"))      //get the key
            {
                QJsonValue value = object.value("kwh");     //get the value of kwh
                if(value.isDouble())
                {
                    //= value.toDouble();             //get kwh and show on the screen

                }
            }

            if(object.contains("bill"))
            {
                QJsonValue value = object.value("bill");    //get the bill
                if(value.isDouble())
                {
                     //= value.toDouble();

                }

            }

        }

    }


}
