#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include "params_defined_dialog.h"
#include "ui_params_defined_dialog.h"
#include "common/widget_utils.h"
#include "common/qobject_utils.h"
#include "common/params_defined.h"


ParamsDefinedDialog::ParamsDefinedDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::ParamsDefinedDialog),row(0),isEdit(false),paramsDefinedEditor((ParamsDefinedEditor*)parent)
{
    ui->setupUi(this);
    init();

    emit ui->cbxType->activated(ui->cbxType->currentIndex());
}

ParamsDefinedDialog::ParamsDefinedDialog(int row,const QJsonObject& jsonData,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamsDefinedDialog),row(row),jsonData(jsonData),isEdit(true),paramsDefinedEditor((ParamsDefinedEditor*)parent)
{
    ui->setupUi(this);

    init();

    QString id = jsonData.value("id").toString();
    QString label = jsonData.value("label").toString();
    QString type = jsonData.value("type").toString();
    QString valueStr = QObjectUtils::getQJsonSimpleShowText(jsonData.value("value"));
    QString optionsStr = QObjectUtils::getQJsonSimpleShowText(jsonData.value("options"));
    bool require = jsonData.value("require").toBool();
    QString describe = jsonData.value("describe").toString();

    ui->editId->setText(id);
    ui->editLabel->setText(label);
    WidgetUtils::setQComboData(ui->cbxType,type);
    ui->editOptions->setText(optionsStr);
    ui->editDefault->setText(valueStr);
    ui ->cbxRequire->setChecked(require);
    ui->areaDescribe->setPlainText(describe);

   emit ui->cbxType->activated(ui->cbxType->currentIndex());
}

ParamsDefinedDialog::~ParamsDefinedDialog()
{
    delete ui;
}


void ParamsDefinedDialog::init()
{
    ui->cbxType->addItem(ParamsDefined::getTypeName(ParamsDefined::Type::text),ParamsDefined::Type::text);
    ui->cbxType->addItem(ParamsDefined::getTypeName(ParamsDefined::Type::area),ParamsDefined::Type::area);
    ui->cbxType->addItem(ParamsDefined::getTypeName(ParamsDefined::Type::select),ParamsDefined::Type::select);
    ui->cbxType->addItem(ParamsDefined::getTypeName(ParamsDefined::Type::checkbox),ParamsDefined::Type::checkbox);

    connect(ui->cbxType,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),this,[=](int index){
        typeChange();
    });
    connect(ui->btnSave,&QPushButton::clicked,[=]{
        saveData();
    });
}

void ParamsDefinedDialog::typeChange()
{
    QString type= WidgetUtils::getQComboData(ui->cbxType);
    if(type=="select" || type=="checkbox"){
        ui->editOptions->setVisible(true);
        ui->labelOptions->setVisible(true);
    }else{
        ui->editOptions->setVisible(false);
        ui->labelOptions->setVisible(false);
    }
}

void ParamsDefinedDialog::saveData()
{
    QString id =  ui->editId->text().trimmed();
    if(id.isEmpty()){
        QMessageBox::warning(NULL,"警告","ID不能为空");
        return;
    }
    QString label =  ui->editLabel->text().trimmed();
    if(label.isEmpty()){
        QMessageBox::warning(NULL,"警告","标题不能为空");
        return;
    }
    QString type =  WidgetUtils::getQComboData(ui->cbxType);
    if(type.isEmpty()){
        QMessageBox::warning(NULL,"警告","类型不能为空");
        return;
    }
    QString valueStr =  ui->editDefault->text().trimmed();
    QJsonValue value;
    if(type== ParamsDefined::Type::checkbox ){
        QJsonArray jsonArr;
        if(!valueStr.isEmpty()){
            jsonArr = QObjectUtils::getQJsonArrayFromQStringList(valueStr.split(",",QString::SkipEmptyParts));
        }
        value = jsonArr;
    }else{
        value = valueStr;
    }

    QJsonArray optionsJsonArr;
    if(type == ParamsDefined::Type::checkbox ||type == ParamsDefined::Type::select  ){
        QString optionsStr = ui->editOptions->text().replace("，",",").replace("：",":");
        if(!optionsStr.isEmpty()){
            QStringList optionsKvs = optionsStr.split(",",QString::SkipEmptyParts);
            foreach (QString optionsKv, optionsKvs) {
                QStringList optionsKvArr = optionsKv.split(":");
                QString name = optionsKvArr[0];
                QString value = name;
                if(optionsKvArr.size()>1){
                    value = optionsKvArr[1];
                }
                QJsonObject item;
                item.insert("name",name);
                item.insert("value",value);
                optionsJsonArr.append(item);
            }
        }
        if(optionsJsonArr.isEmpty()){
            QMessageBox::warning(NULL,"警告","待选项不能为空");
            return;
        }
    }

    bool require = ui->cbxRequire->isChecked();
    QString describe = ui->areaDescribe->toPlainText().trimmed();

    QJsonObject jsonData;
    jsonData.insert("id",id);
    jsonData.insert("label",label);
    jsonData.insert("type",type);
    jsonData.insert("value",value);
    jsonData.insert("require",require);
    jsonData.insert("describe",describe);
    jsonData.insert("options",optionsJsonArr);
    if(isEdit){
        this->paramsDefinedEditor->setRowData(this->row,jsonData);
    }else{
        this->paramsDefinedEditor->appendRowData(jsonData);
    }
    this->close();
}
