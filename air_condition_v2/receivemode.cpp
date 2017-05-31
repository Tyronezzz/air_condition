#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonValue>

//Purpose: receive mode and temperature from host
void receivemode(QByteArray byteArray)
{
    QJsonParseError jsonError;

    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
    if(!document.isNull() && (jsonError.error==QJsonParseError::NoError) )
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();     //convert jsondoc into object
            if(object.contains("workingmode"))      //get the key
            {
                QJsonValue value = object.value("workingmode");     //get the value of workingmode
                if(value.isString())
                {
                    //= value.toString();             //set the mode and show on the screen

                }
            }

            if(object.contains("defaulttemp"))
            {
                QJsonValue value = object.value("defaulttemp");    //get the default temp


            }

        }

    }
}
