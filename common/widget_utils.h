#ifndef WIDGET_UTILS_H
#define WIDGET_UTILS_H

#include<QWidget>
#include<QString>
#include<QJsonArray>
#include<QJsonObject>
#include<QComboBox>

namespace WidgetUtils
{

void setQComboOptions(QComboBox* cbx,QJsonArray options);
bool setQComboData(QComboBox* cbx,QVariant data);
QString getQComboData(QComboBox* cbx);

void setQCheckBoxOptions(QWidget *parent, QJsonArray options);
void setQCheckBoxs(QWidget* parent,QJsonArray values);
QJsonArray getQCheckBoxs(QWidget* parent);

}
#endif // WIDGET_UTILS_H
