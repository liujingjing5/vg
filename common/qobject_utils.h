#ifndef QOBJECT_UTILS_H
#define QOBJECT_UTILS_H

#include<QStringList>
#include<QJsonArray>

namespace QObjectUtils
{
/**
  支持字符串 字符串数组 name&value组合的数组
 * @brief getQJsonSimpleShowText
 * @param jsonVar
 * @return
 */
QString getQJsonSimpleShowText(QJsonValue jsonVar);
QStringList getQStringListFromQJsonArray(QJsonArray jsonArr);
QJsonArray getQJsonArrayFromQStringList(QStringList stringList);

QString abbreviate(const QString& str,int maxlen);

}

#endif // QOBJECT_UTILS_H
