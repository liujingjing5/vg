#include "params_defined_editor.h"
#include "ui_params_defined_editor.h"
#include <QMessageBox>
#include <QtGlobal>
#include <QTime>
#include <QRegularExpression>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QModelIndex>
#include <QEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QByteArray>
#include <QDataStream>
#include <QMap>
#include <QTableWidgetItem>
#include <QTimer>

#include "common/utils.h"
#include "params_defined_dialog.h"
#include "common/qobject_utils.h"
#include "common/params_defined.h"


ParamsDefinedEditor::ParamsDefinedEditor(const QString& filePath,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamsDefinedEditor),
    filePath(filePath),dirty(false)
{
    ui->setupUi(this);
//    QMenu* menuAdd = new QMenu(this);
//    QAction* menuAddTextAction = new QAction(menuAdd);
//    menuAddTextAction->setText(ParamsDefined::getTypeName(ParamsDefined::Type::text));
//    menuAdd->addAction(menuAddTextAction);
//    QAction* menuAddAreaAction = new QAction(menuAdd);
//    menuAddAreaAction->setText(ParamsDefined::getTypeName(ParamsDefined::Type::area));
//    menuAdd->addAction(menuAddAreaAction);
//    QAction* menuAddSelectAction = new QAction(menuAdd);
//    menuAddSelectAction->setText(ParamsDefined::getTypeName(ParamsDefined::Type::select));
//    menuAdd->addAction(menuAddSelectAction);
//    QAction* menuAddCheckboxAction = new QAction(menuAdd);
//    menuAddCheckboxAction->setText(ParamsDefined::getTypeName(ParamsDefined::Type::checkbox));
//    menuAdd->addAction(menuAddCheckboxAction);
//    ui->btnAddParams->setMenu(menuAdd);

    connect(ui->btnAddParams, &QPushButton::clicked,[=]{
        ParamsDefinedDialog* pd = new ParamsDefinedDialog(this);
        pd->exec();
    });
    connect(ui->btnDeleteParams, &QPushButton::clicked,[=]{
        int deleteRow = ui->tableParams->currentRow();
        if(deleteRow==-1){
            QMessageBox::warning(NULL,"警告","请先选中参数行");
            return;
        }
        deleteRowData(deleteRow);
    });


//    ui->tableParams->setRowCount(1);
    ui->tableParams->setColumnCount(7);
    QStringList header;
    header <<"ID"<<"标题"<<"类型"<<"必填"<<"待选项"<<"默认值"<<"说明";   //表头
    ui->tableParams->setHorizontalHeaderLabels(header);
    ui->tableParams->horizontalHeader()->setStretchLastSection(true);
    ui->tableParams->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableParams->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableParams->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableParams->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
    ui->tableParams->setWordWrap(false);

    //注册事件过滤器
    ui->tableParams->viewport()->installEventFilter(this);
       ////拖放使能
    ui->tableParams->setDragDropMode(QAbstractItemView::DragDrop);
    ui->tableParams->setDragEnabled(true);
    ui->tableParams->setAcceptDrops(true);

    QString content = utils::readFileToQString(filePath);

    QJsonDocument jsondoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonArray jsonArr =  jsondoc.array();

    QAbstractItemModel* model = ui->tableParams->model();
    foreach (QJsonValue jsonVar, jsonArr) {
        QJsonObject itemJsonObj = jsonVar.toObject();
       int row = model->rowCount();
        model->insertRow(row);
        setRowData(row,itemJsonObj);
    }


    connect(ui->tableParams,QTableWidget::doubleClicked,[=,model](const QModelIndex &index){
            QModelIndex cindex= model->index(index.row(), 0);
            QJsonObject itemJsonObj =  QJsonDocument::fromJson(model->data(cindex,Qt::UserRole).toByteArray()).object();
            ParamsDefinedDialog* pd = new ParamsDefinedDialog(index.row(),itemJsonObj,this);
            pd->exec();
    });

}

ParamsDefinedEditor::~ParamsDefinedEditor()
{
    delete ui;
}

QString ParamsDefinedEditor::editorFilePath()
{
    return filePath;
}

bool ParamsDefinedEditor::editorClose()
{
    qDebug() <<"editorClose:" << this->dirty;
    if(dirty){
        QString fileName =  QFileInfo(filePath).fileName();
        int res = QMessageBox::warning(this,"警告","关闭前，是否保存 "+fileName+" 文件？",QMessageBox::Yes |QMessageBox::No| QMessageBox::Cancel, QMessageBox::Cancel);
        if(res == QMessageBox::Yes){
            editorSave();
        }else if(res == QMessageBox::No){

        }else{
            return false;
        }
    }
    return true;
}

bool ParamsDefinedEditor::editorSave()
{
    QFileInfo fileInfo(filePath);
    if(!fileInfo.exists()){
        int res = QMessageBox::warning(this,"警告","文件不存在，是否保存 "+fileInfo.fileName()+" 文件？",QMessageBox::Yes |QMessageBox::No, QMessageBox::No);
        if(res == QMessageBox::Yes){

        }else{
            return false;
        }
    }
    QAbstractItemModel* model = ui->tableParams->model();
    int len = model->rowCount();
    QJsonArray jsonarr;
    for(int i=0;i<len;i++){
        QModelIndex itemIndex= model->index(i, 0);
        QJsonObject itemObj = QJsonDocument::fromJson(model->data(itemIndex,Qt::UserRole).toByteArray()).object();
        jsonarr.append(itemObj);
    }
    QString dataStr = QString(QJsonDocument(jsonarr).toJson());
    utils::writeQStringToFile(dataStr,filePath);
    return true;
}

void ParamsDefinedEditor::appendRowData(QJsonObject jsonObj)
{
    QAbstractItemModel* model = ui->tableParams->model();
    int indexNum = model->rowCount();
    model->insertRow(indexNum);
    setRowData(indexNum,jsonObj);
}

void ParamsDefinedEditor::setRowData(int row, QJsonObject itemJsonObj)
{
    QAbstractItemModel* model = ui->tableParams->model();

    QString id = itemJsonObj.value("id").toString();
    QString label = itemJsonObj.value("label").toString();
    QString type = ParamsDefined::getTypeName(itemJsonObj.value("type").toString());
    QString options = QObjectUtils::getQJsonSimpleShowText(itemJsonObj.value("options"));
    QString value = QObjectUtils::getQJsonSimpleShowText(itemJsonObj.value("value"));
    QString require = itemJsonObj.value("require").toBool()?"是":"";
    QString describe = itemJsonObj.value("describe").toString();

    QModelIndex index= model->index(row, 0);
    model->setData(index,QJsonDocument(itemJsonObj).toJson(),Qt::UserRole);
    model->setData(model->index(row, 0),id);
    model->setData(model->index(row, 1),label);
    model->setData(model->index(row, 2),type);
    model->setData(model->index(row, 3),require);
    model->setData(model->index(row, 4),options);
    model->setData(model->index(row, 5),value);
    model->setData(model->index(row, 6),describe);

    QTimer::singleShot(1,[=]{
        editorSave();
    });

}
void ParamsDefinedEditor::deleteRowData(int row)
{
    if(row>=0 && row < ui->tableParams->rowCount()){
        QAbstractItemModel* model = ui->tableParams->model();
        model->removeRow(row);
        QTimer::singleShot(1,[=]{
            editorSave();
        });
    }
}

void ParamsDefinedEditor::dragDrop(int fromRow,int toRow){
    QAbstractItemModel* model = ui->tableParams->model();

    QModelIndex fromIndex= model->index(fromRow, 0);
    QJsonObject fromJsonObj = QJsonDocument::fromJson(model->data(fromIndex,Qt::UserRole).toByteArray()).object();
    model->removeRow(fromRow);

//    QModelIndex toIndex= model->index(toRow, 0);
//                QJsonObject toJsonObj = QJsonDocument::fromJson(model->data(toIndex,Qt::UserRole).toByteArray()).object();

    model->insertRow(toRow);
    setRowData(toRow,fromJsonObj);

    ui->tableParams->selectRow(toRow);

}

bool ParamsDefinedEditor::eventFilter(QObject *obj, QEvent *eve)
{
    if(obj == ui->tableParams->viewport())
    {
        if(eve->type() == QEvent::Drop)
        {
            const QMimeData *mime = ((QDropEvent*)eve)->mimeData();
            QByteArray encodedata = mime->data("application/x-qabstractitemmodeldatalist");
            if (encodedata.isEmpty())
            {
                return false;
            }
            QDataStream stream(&encodedata, QIODevice::ReadOnly);
            while (!stream.atEnd())
            {
                int fromRow;
                ///拖的row和col
                stream >> fromRow;
                QTableWidgetItem* pDropItem = ui->tableParams->itemAt(((QDropEvent*)eve)->pos());
                if(!pDropItem)
                {
                    return true;
                }
                int toRow = pDropItem->row();
                //放的row
                if(toRow == fromRow)
                {
                    return true;
                }
                ///自己的实现:
                dragDrop(fromRow,toRow);
                return true;    //不要交给系统处理，否则他会给你新增一行
            }
        }else
        {
            return QWidget::eventFilter(obj,eve);
        }
    }
    return QWidget::eventFilter(obj,eve);
}

