#include "qobject_utils.h"
#include<QJsonValue>
#include<QJsonObject>

namespace QObjectUtils
{


QString getQJsonSimpleShowText(QJsonValue jsonVar)
{
    if(jsonVar.isArray()){
        QJsonArray options = jsonVar.toArray();
        QStringList optionStrList;
        foreach (QJsonValue jsonVar, options) {
            if(jsonVar.isObject()){
                optionStrList.append(getQJsonSimpleShowText(jsonVar));
            }else{
                optionStrList.append(jsonVar.toString());
            }
        }
        QString optionsStr = optionStrList.join(",");
        return optionsStr;
    }if(jsonVar.isObject()){
        QJsonObject jsonObj = jsonVar.toObject();
        QString key = jsonObj.value("name").toString();
        QString value = jsonObj.value("value").toString();
        return key+":"+value;
    }else{
        return jsonVar.toString();
    }
}

QStringList getQStringListFromQJsonArray(QJsonArray jsonArr)
{
    QStringList slist;
    foreach (QJsonValue jsonVar, jsonArr) {
        slist.append(jsonVar.toString());
    }
    return slist;
}

QJsonArray getQJsonArrayFromQStringList(QStringList stringList)
{
    QJsonArray jsonArr;
    foreach (QString str, stringList) {
        jsonArr.append(str);
    }
    return jsonArr;
}

QString abbreviate(const QString &str, int maxlen)
{
    int len = str.length();
    if(len > maxlen){
        str.mid(0,maxlen);
        return str+"...";
    }else{
        return str;
    }
}





}
