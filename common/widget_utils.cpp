#include "widget_utils.h"
#include<QCheckBox>
#include<QLayout>
#include<QWidget>

namespace WidgetUtils
{

void setQComboOptions(QComboBox *cbx, QJsonArray options)
{
    foreach (QJsonValue option, options) {
        QJsonObject item = option.toObject();
        QString name = item.value("name").toString();
        QString value = item.value("value").toString();
        cbx->addItem(name,value);
    }
}
bool setQComboData(QComboBox *cbx, QVariant data){
    bool isOk=false;
    for(int i=0;i<cbx->count();i++){
        QVariant qv = cbx->itemData(i);
        if(qv == data){
            cbx->setCurrentIndex(i);
            isOk=true;
            break;
        }
    };
    return isOk;
}
QString getQComboData(QComboBox *cbx)
{
    return cbx->currentData().toString();
}


void setQCheckBoxOptions(QWidget *parent, QJsonArray options)
{
    foreach (QJsonValue option, options) {
        QJsonObject item = option.toObject();
        QString name = item.value("name").toString();
        QString value = item.value("value").toString();
        QCheckBox *radio1 = new QCheckBox(parent);
        radio1->setText(name);
        radio1->setProperty("value",value);
        parent->layout()->addWidget(radio1);
    }
}
void setQCheckBoxs(QWidget *parent, QJsonArray values)
{
    foreach (QObject* obj, parent->children()) {
        if(QString("QCheckBox") == obj->metaObject()->className()){
            QCheckBox* cb = (QCheckBox* )obj;
            QString v = cb->property("value").toString();
            if(values.contains(v)){
                cb->setChecked(true);
            }else{
                cb->setChecked(false);
            }
        }
    }
}
QJsonArray getQCheckBoxs(QWidget *parent)
{
    QJsonArray qarr;
    foreach (QObject* obj, parent->children()) {
        QCheckBox* cb = (QCheckBox* )obj;
        if(cb->isChecked()){
            QString v = cb->property("value").toString();
            qarr.append(v);
        }
    }
    return qarr;
}

}
