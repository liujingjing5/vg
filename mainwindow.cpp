#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QDropEvent>
#include <QEvent>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMimeData>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QThreadPool>
#include <QTextEdit>
#include <QCheckBox>
#include <QInputDialog>
#include <QTimer>
#include <QDesktopServices>
#include <common/flowlayout.h>
#include "common/sg.h"
#include "common/utils.h"
#include "common/template_manager.h"
#include "common/script_manager.h"
#include "project_maker_window.h"
#include "common/file_type/file_type_utils.h"


namespace PrjItemEnum {
    const static char* prop = "prop";
    const static char* id = "id";
    const static char* label = "label";
    const static char* type = "type";
    const static char* require = "require";
    const static char* value = "value";
    const static char* describe = "describe";
}
namespace PrjItemTypeEnum{
    const static char* text = "text";
    const static char* area = "area";
    const static char* select = "select";
    const static char* checkbox = "checkbox";
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textTargetPath->setAcceptDrops(true);
    ui->textTargetPath->setDragEnabled(false);

    QThreadPool::globalInstance()->setMaxThreadCount(10);
    sg<TemplateManager>::get()->init(this);
    sg<ScriptManager>::get()->init(this);


//    this->setAcceptDrops(true);

    connect(ui->actionExit,&QAction::triggered,this,[=]{
       this->close();
    });
    connect(ui->btnTargetPath,&QToolButton::clicked,this,&MainWindow::handleBtnTargetPath );
    connect(ui->actionProjectCreate,&QAction::triggered,this,&MainWindow::handleProjectCreate);
    connect(ui->actionProjectModify,&QAction::triggered,this,&MainWindow::handleProjectModify);
    connect(ui->actionProjectDelete,&QAction::triggered,this,&MainWindow::handleProjectDelete);
    connect(ui->actionProjectInfo,&QAction::triggered,this,&MainWindow::handleProjectInfo);
    connect(ui->actionAboutQt,&QAction::triggered,this,[=]{
        QMessageBox::aboutQt(this, tr("aboutQt Msg"));
    });
    connect(ui->actionAboutVG,&QAction::triggered,this,[=]{
        QMessageBox::about(this,"关于Visual Generator","Visual Generator是一个可视化的模版编辑器，\r\n通过自定义的参数输入界面 -> 过滤器脚本自由变换 -> 模版生成\r\n方便高效的生成项目文件。");
    });
    connect(ui->cbxProject,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),this,&MainWindow::handleCbxProjectChange);

    QMenu* menuBtnCodeGen = new QMenu(this);
    QAction* menuBtnCodeGenCreateAction = new QAction(menuBtnCodeGen);
    QAction* menuBtnCodeGenCreateAndOpenAction = new QAction(menuBtnCodeGen);
    menuBtnCodeGenCreateAction->setText("生成");
    menuBtnCodeGenCreateAndOpenAction->setText("生成并查看");
    menuBtnCodeGen->addAction(menuBtnCodeGenCreateAction);
    menuBtnCodeGen->addAction(menuBtnCodeGenCreateAndOpenAction);
    connect(menuBtnCodeGenCreateAction, &QAction::triggered, this,[=](bool checked){
        handleBtnCodeGen(false);
    });
    connect(menuBtnCodeGenCreateAndOpenAction, &QAction::triggered, this,[=](bool checked){
        handleBtnCodeGen(true);
    });
    ui->btnCodeGen->setMenu(menuBtnCodeGen);
    connect(ui->btnCodeGen,&QToolButton::clicked,this,[=](bool checked){
        handleBtnCodeGen(false);
    });

    ui->textTargetPath->installEventFilter(this);

//    this->setWindowFlags(this->windowFlags()  |   Qt::WindowStaysOnTopHint);
    QTimer::singleShot(100, this,[=]{
        this->refreshCbxProject();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
//    qDebug() << "event  "<<event  ;
    if (event->type() == QEvent::DragEnter) {
        QDragEnterEvent *e = static_cast<QDragEnterEvent *>(event);
        e->acceptProposedAction();
        return true;
    }
    if (event->type() == QEvent::Drop) {
        QDropEvent *e = static_cast<QDropEvent *>(event);
        const QMimeData *mimeData = e->mimeData();//获取mimie数据
        const QList<QUrl> urls = mimeData->urls();
            if(!urls.isEmpty())
            {
                QFileInfo fileInfo(urls.at(0).toLocalFile());
                if(fileInfo.isDir()){
                    ui->textTargetPath->setText(fileInfo.filePath());
                }
            }
            return true;
    }
    return false;
}
void MainWindow::refreshCbxProject(const QString& oldPrjName )
{
    int oldIndex = ui->cbxProject->currentIndex();
    ui->cbxProject->clear();
    QDir dataDir(".");
    qDebug()<< dataDir.absolutePath();
    if(!dataDir.exists("data")){
        qDebug()<< dataDir.mkpath("data");
        qDebug()<< dataDir.mkpath("data/projects");
    }
    QDir projectsDir("data/projects");
    projectsDir.setFilter(QDir::Dirs);
    projectsDir.setSorting(QDir::SortFlag::Name);
    QFileInfoList projectDirList = projectsDir.entryInfoList();
    foreach (QFileInfo projectDir, projectDirList) {
        if(projectDir.fileName()=="."||projectDir.fileName()==".."){
            continue;
        }
        ui->cbxProject->addItem(projectDir.fileName());
    }
    if(oldPrjName.isEmpty()){
        int count = ui->cbxProject->count();
        if(count > 0){
            oldIndex = (oldIndex>=0)? oldIndex:0;
            oldIndex = (oldIndex < count)? oldIndex: (count-1);
        }else{
            oldIndex =-1;
        }
        ui->cbxProject->setCurrentIndex(oldIndex);
    }else{
        ui->cbxProject->setCurrentText(oldPrjName);
    }
    emit ui->cbxProject->activated(ui->cbxProject->currentIndex());
}

void MainWindow::handleBtnTargetPath()
{
   QString oldPath = ui->textTargetPath->text();
   QString prePath = oldPath.isEmpty()?QDir::home().absolutePath() :oldPath;
   QString targetPath = QFileDialog::getExistingDirectory(this,"请选择",prePath,QFileDialog::ShowDirsOnly);
   if(!targetPath.isEmpty()){
       ui->textTargetPath->setText(targetPath);
   }
}

void MainWindow::handleCbxProjectChange(int index)
{
    QFormLayout* layout = (QFormLayout*)ui->groupVarsetting->layout();
    int oldRowLen = layout->rowCount();
    for(int i=0;i<oldRowLen;i++){
        layout->removeRow(0);
    }

    QString text =  ui->cbxProject->itemText(index);
    if(!text.isEmpty()){
        QFile uijs("data/projects/"+text+"/ui.json");
        if(!uijs.exists()){
            QMessageBox::critical(NULL, "错误", "ui配置不存在，方案文件可能已损坏");
        }else{
    //        QMessageBox::about(NULL, "提示", "当前是："+text);
            if(!uijs.open(QIODevice::ReadOnly | QIODevice::Text)){
                QMessageBox::critical(NULL, "错误", "ui配置读取失败，方案文件可能已损坏");
            }else{
                QByteArray data = uijs.readAll();
                qDebug()<< QString(data);
                QJsonParseError error;
                QJsonDocument doc = QJsonDocument::fromJson(data,&error);
                if(error.error != QJsonParseError::NoError){
                    QMessageBox::critical(NULL,"参数输入界面定义错误", error.errorString());
                    return;
                }
                qDebug()<< QString(doc.toJson());
                QJsonArray items = doc.array();
                foreach (QJsonValue item, items) {
                    qDebug() << item;
                    QJsonObject itemObj = item.toObject();
                    QString id = itemObj.value(PrjItemEnum::id).toString();
                    QString label = itemObj.value(PrjItemEnum::label).toString();
                    bool require = itemObj.value(PrjItemEnum::require).toBool();
                    QString type = itemObj.value(PrjItemEnum::type).toString();
                    QJsonValue value = itemObj.value(PrjItemEnum::value);
                    QString describe = itemObj.value(PrjItemEnum::describe).toString();
                    QWidget * paramItemWidget=NULL;
                    if(type==PrjItemTypeEnum::text){
                        QLineEdit * paramItem = new QLineEdit(ui->groupVarsetting);
                        paramItem->setObjectName("__ui_"+id);
                        if(!value.isNull()){
                            paramItem->setText(value.toString());
                        }
                        paramItem->setProperty(PrjItemEnum::prop,QVariant(itemObj));
                        paramItemWidget = paramItem;
                    }else if(type==PrjItemTypeEnum::area){
                        QTextEdit * paramItem = new QTextEdit(ui->groupVarsetting);
                        paramItem->setAcceptRichText(false);
                        paramItem->setFixedHeight(100);
                        paramItem->setObjectName("__ui_"+id);
                        if(!value.isNull()){
                            paramItem->setText(value.toString());
                        }
                        paramItem->setProperty(PrjItemEnum::prop,QVariant(itemObj));
                        paramItemWidget = paramItem;
                    }else if(type==PrjItemTypeEnum::select){
                        QComboBox * paramItem = new QComboBox(ui->groupVarsetting);
                        paramItem->setObjectName("__ui_"+id);
                        if(!value.isNull()){
    //                        paramItem->setText(value);
                        }
                        paramItem->setProperty(PrjItemEnum::prop,QVariant(itemObj));
                        QJsonArray options = itemObj.value("options").toArray();

                        foreach (QJsonValue option,options) {
                            QString name =  option["name"].toString();
                            QString value =  option["value"].toString();
                            paramItem->addItem(name,QVariant(value));
                        }
                        paramItemWidget = paramItem;
                    }else if(type==PrjItemTypeEnum::checkbox){
                        QGroupBox * paramItem = new QGroupBox(ui->groupVarsetting);
                        paramItem->setObjectName("__ui_"+id);
                        if(!value.isNull()){
    //                        paramItem->setText(value);
                        }
                        paramItem->setProperty(PrjItemEnum::prop,QVariant(itemObj));
                        QJsonArray options = itemObj.value("options").toArray();

                        FlowLayout *radioLayout = new FlowLayout;
                        paramItem->setLayout(radioLayout);
                        // 三个单选钮为一个群组
                        foreach (QJsonValue option,options) {
                            QString name =  option["name"].toString();
                            QString value =  option["value"].toString();
                            QCheckBox *radio1 = new QCheckBox(paramItem);
                            radio1->setText(name);
                            radio1->setProperty("value",value);
                            radioLayout->addWidget(radio1);
                        }
    //                    radioLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
                        paramItemWidget = paramItem;
                    }

                    if(paramItemWidget!=NULL){
                        QLabel* qlabel = new QLabel(ui->groupVarsetting);
                        qlabel->setToolTip("["+id+"] "+describe);
                        qlabel->setText((require?"*":"")+label);
                        layout->addRow(qlabel,paramItemWidget);
                    }
                }
            };
        }
    }
}

void MainWindow::refreshProjectParams(){
    int index = ui->cbxProject->currentIndex();
    if(index!=-1){
        bool isOk;
        QJsonObject jsonObj = getProjectParamsValues(false,&isOk);
        if(isOk){
            handleCbxProjectChange(index);
            setProjectParamsValues(jsonObj);
        }
    }
}

QJsonObject MainWindow::getProjectParamsValues(bool needRequire,bool* isOk){
    if(isOk!=NULL){
       *isOk=false;
    }
    QJsonObject jsonObj;
    QFormLayout* layout = (QFormLayout*)ui->groupVarsetting->layout();
    int rowlen = layout->count();
    for(int i=0;i<rowlen;i++){
        QLayoutItem* qitem =  layout->itemAt(i);
        QWidget* qwidget =  qitem->widget();
        QVariant paramSetting =  qwidget->property(PrjItemEnum::prop);
        if(!paramSetting.isNull()){
            QJsonObject prop =  paramSetting.toJsonObject();
            QString id = prop.value(PrjItemEnum::id).toString();
            QString label = prop.value(PrjItemEnum::label).toString();
            bool require = prop.value(PrjItemEnum::require).toBool();
            QString type = prop.value(PrjItemEnum::type).toString();
            if(type ==  PrjItemTypeEnum::text){
                QString value = ((QLineEdit*)qwidget)->text();
                jsonObj.insert(id,value);
                if(require && value.isEmpty() && needRequire){
                    QMessageBox::critical(NULL,"错误",label+"必填");
                    return QJsonObject();
                }
            }else if(type ==  PrjItemTypeEnum::area){
                QString value = ((QTextEdit*)qwidget)->toPlainText();
                jsonObj.insert(id,value);
                if(require && value.isEmpty() && needRequire){
                    QMessageBox::critical(NULL,"错误",label+"必填");
                    return QJsonObject();
                }
            }else if(type ==  PrjItemTypeEnum::select){
                QString value = ((QComboBox*)qwidget)->currentData().toString();
                jsonObj.insert(id,value);
                if(require && value.isEmpty() && needRequire){
                    QMessageBox::critical(NULL,"错误",label+"必填");
                    return QJsonObject();
                }
            }else if(type ==  PrjItemTypeEnum::checkbox){
                QGroupBox* qroup = (QGroupBox*)qwidget;
                QJsonArray qarr;
                foreach (QObject* obj, qroup->children()) {
                    QCheckBox* cb = (QCheckBox* )obj;
                    if(cb->isChecked()){
                        QString v = cb->property("value").toString();
                        qarr.append(v);
                    }
                }
                jsonObj.insert(id,qarr);
                if(require && qarr.isEmpty() && needRequire){
                    QMessageBox::critical(NULL,"错误",label+"必填");
                    return QJsonObject();
                }
            }
        }
    }
    if(isOk!=NULL){
       *isOk=true;
    }
    return jsonObj;
}

void MainWindow::setProjectParamsValues(const QJsonObject& jsonObj){
    QFormLayout* layout = (QFormLayout*)ui->groupVarsetting->layout();
    int rowlen = layout->count();
    for(int i=0;i<rowlen;i++){
        QLayoutItem* qitem =  layout->itemAt(i);
        QWidget* qwidget =  qitem->widget();
        QVariant paramSetting =  qwidget->property(PrjItemEnum::prop);
        if(!paramSetting.isNull()){
            QJsonObject prop =  paramSetting.toJsonObject();
            QString id = prop.value(PrjItemEnum::id).toString();
            QString label = prop.value(PrjItemEnum::label).toString();
            bool require = prop.value(PrjItemEnum::require).toBool();
            QString type = prop.value(PrjItemEnum::type).toString();
            if(type ==  PrjItemTypeEnum::text){
                if(jsonObj.contains(id)){
                    QString value = jsonObj.value(id).toString();
                    ((QLineEdit*)qwidget)->setText(value);
                }
            }else if(type ==  PrjItemTypeEnum::area){
                if(jsonObj.contains(id)){
                    QString value = jsonObj.value(id).toString();
                    ((QTextEdit*)qwidget)->setPlainText(value);
                }
            }else if(type ==  PrjItemTypeEnum::select){
                if(jsonObj.contains(id)){
                    QString value = jsonObj.value(id).toString();
                    QComboBox* cbx = ((QComboBox*)qwidget);
                    for(int i=0;i<cbx->count();i++){
                        QVariant qv = cbx->itemData(i);
                        if(qv == value){
                            cbx->setCurrentIndex(i);
                            break;
                        }
                    };
                }
            }else if(type ==  PrjItemTypeEnum::checkbox){
                if(jsonObj.contains(id)){
                    QJsonArray values = jsonObj.value(id).toArray();
                    QGroupBox* qroup = (QGroupBox*)qwidget;
                    foreach (QObject* obj, qroup->children()) {
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
            }
        }
    }
}


void MainWindow::handleBtnCodeGen(bool show)
{

    QString targetPathStr = this->ui->textTargetPath->text();
    QString prjName = this->ui->cbxProject->currentText();
    if(prjName.isEmpty()){
        QMessageBox::critical(NULL,"错误","生成方案不能为空");
        return;
    }
    if(targetPathStr.isEmpty()){
        QMessageBox::critical(NULL,"错误","生成目录不能为空");
        return;
    }
        QDir targetDir(targetPathStr);
        if(!targetDir.exists()){
            targetDir.mkdir(".");
        }
        bool isOk;
        QJsonObject jsonObj = getProjectParamsValues(true,&isOk);
        if(!isOk){
            return;
        }
        QJsonDocument jsonDoc(jsonObj);
        QJsonDocument jsonDocPlus = sg<ScriptManager>::get()->filter(prjName,jsonDoc,&isOk);
        if(!isOk){
            QMessageBox::critical(NULL,"过滤器错误", jsonDocPlus.object().value("message").toString());
            return;
        }
        QString paramsStr(jsonDocPlus.toJson());
        qDebug() <<"paramsStr:"<< paramsStr;


        QString templateDirStr = "data/projects/"+prjName+"/template";
        QDir templateDir(templateDirStr);

        QFileInfoList fileInfoList =  utils::listFileInfoList(templateDirStr);
        foreach (QFileInfo tempfileInfo, fileInfoList) {
            bool renderOk;
            if(tempfileInfo.isFile()){
                QString tempfileLitePath = tempfileInfo.filePath().replace(templateDir.path()+"/","");
                QString targetFile = targetDir.filePath(tempfileLitePath);
                targetFile =  sg<TemplateManager>::get()->renderText(targetFile,jsonDocPlus,&renderOk);
                if(!renderOk){
                    QMessageBox::critical(NULL,"路径模版渲染错误",tempfileInfo.filePath()+":\r\n"+targetFile);
                    return;
                }

                FileTypeUtils::FileType fileType = FileTypeUtils::findFileTypeByFilePath(tempfileInfo.filePath());
                if(fileType.type=="text"){
                    QString srcStr = utils::readFileToQString(tempfileInfo.filePath(),fileType.characterSet);
                    QString result = sg<TemplateManager>::get()->renderText(srcStr,jsonDocPlus,&renderOk);
                    if(!renderOk){
                        QMessageBox::critical(NULL,"文件模版渲染错误",tempfileInfo.filePath()+":\r\n"+result);
                        return;
                    }
                    utils::writeQStringToFile(result,targetFile,fileType.characterSet);
    //                QString targetDirStr = tempfileInfo.dir().path().replace(templateDir.path(),"");
    //                targetDir.mkpath(targetDirStr);
                    qDebug()<<"生成文件:"<<targetFile;
                }else{
                    utils::copy(tempfileInfo.filePath(),targetFile,true);
                    qDebug()<<"复制文件:"<<targetFile;
                }
            }else if(tempfileInfo.isDir()){
                QString tempNodeDirLitePath = tempfileInfo.filePath().replace(templateDir.path()+"/","");
                QString targetNodeDir = targetDir.filePath(tempNodeDirLitePath);
                targetNodeDir =  sg<TemplateManager>::get()->renderText(targetNodeDir,jsonDocPlus,&renderOk);
                if(!renderOk){
                    QMessageBox::critical(NULL,"路径模版渲染错误",tempfileInfo.filePath()+":\r\n"+targetNodeDir);
                    return;
                }
                QDir(targetNodeDir).mkpath(".");
                qDebug()<<"生成目录:"<<targetNodeDir;
            }
        }
        QMessageBox::information(NULL,"消息","方案已生成");
        if(show){
            QDesktopServices::openUrl(QUrl("file:///"+targetDir.path(), QUrl::TolerantMode));
        }
//        templateDir.setFilter( QDir::Files | QDir::Dirs | QDir::NoSymLinks );
//        templateDir.setSorting( QDir::Size | QDir::Reversed );
//        const QFileInfoList templist =templateDir.entryInfoList();
//        foreach (QFileInfo tempfileInfo, templist) {
//            qDebug()<<    tempfileInfo;
//        }


//    QString rtxt = sg<TemplateManager>::get()->renderText("<div>[% if age == 32 %][[name]][% else %][[name]]不对[% endif %]</div>","{\"name\":\"Jake\",\"age\":31}");
//    qDebug() << "QScriptEngine:" << rtxt;



}

void MainWindow::handleProjectCreate()
{
    bool isPrjNameOK;
    QString prjName = QInputDialog::getText(this, "方案名称","请输入方案名称",QLineEdit::Normal, "", &isPrjNameOK).trimmed();
    if(isPrjNameOK) {
        if(prjName.isEmpty()){
            QMessageBox::warning(this, "警告","方案名称不能为空");
            return;
        }
    }else{
        return;
    }

    QString uiStr = utils::readFileToQString(":/asserts/project/ui.json");
    QString filterStr = utils::readFileToQString(":/asserts/project/filter.js");
    QString readmeStr = utils::readFileToQString(":/asserts/project/readme.md");

    QFileInfo projectFileInfo(QString("data/projects/")+prjName);
    if(projectFileInfo.exists()){
        QMessageBox::warning(this,"错误","方案已存在");
        return;
    }
    QDir projectDir(projectFileInfo.filePath());
    projectDir.mkpath("template");

    utils::writeQStringToFile(uiStr,projectDir.filePath("ui.json"));
    utils::writeQStringToFile(filterStr,projectDir.filePath("filter.js"));
    utils::writeQStringToFile(readmeStr,projectDir.filePath("readme.md"));

    refreshCbxProject(prjName);

    ProjectMakerWindow* pw = new ProjectMakerWindow(prjName,this);
    pw->show();
}

void MainWindow::handleProjectModify()
{
    QString prjName = ui->cbxProject->currentText();
    if(!prjName.isEmpty()){
        QFileInfo projectFileInfo(QString("data/projects/")+prjName);
        if(!projectFileInfo.exists()){
            QDir(projectFileInfo.filePath()).mkdir(".");
        }
        QDir projectDir(projectFileInfo.filePath());
        if(!projectDir.exists("template")){
            QDir(projectFileInfo.filePath()+("/template")).mkdir(".");
        }
        if(!projectDir.exists("ui.json")){
            QString uiStr = utils::readFileToQString(":/asserts/project/ui.json");
            utils::writeQStringToFile(uiStr,projectDir.filePath("ui.json"));
        }
        if(!projectDir.exists("filter.js")){
            QString filterStr = utils::readFileToQString(":/asserts/project/filter.js");
            utils::writeQStringToFile(filterStr,projectDir.filePath("filter.js"));
        }
        if(!projectDir.exists("readme.md")){
            QString readmeStr = utils::readFileToQString(":/asserts/project/readme.md");
            utils::writeQStringToFile(readmeStr,projectDir.filePath("readme.md"));
        }

        ProjectMakerWindow* pw = new ProjectMakerWindow(ui->cbxProject->currentText(),this);
        pw->show();
    }else{
        QMessageBox::critical(NULL, "错误", "没有方案");
    }
}

void MainWindow::handleProjectDelete()
{
    QString oldPrjName = ui->cbxProject->currentText();
    if(!oldPrjName.isEmpty()){
        int res = QMessageBox::information(NULL, "提示", "方案删除",QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            bool rmres = utils::remove(QString("data/projects/")+oldPrjName);
            if(rmres){
                refreshCbxProject();
            }else{
                QMessageBox::critical(NULL, "错误", "删除失败");
            }
        }
    }else{
        QMessageBox::critical(NULL, "错误", "没有方案");
    }

}

void MainWindow::handleProjectInfo()
{
    QString prjName =  ui->cbxProject->currentText();
    if(!prjName.isEmpty()){
        QString readMePath = ("data/projects/"+prjName+"/readme.md");
        QString readMeStr = utils::readFileToQString(readMePath);
        if(!readMeStr.isEmpty()){
            QMessageBox::about(NULL,"方案详情",readMeStr);
        }else{
            QMessageBox::about(NULL,"方案详情","无");
        }
    }else{
        QMessageBox::critical(NULL, "错误", "没有方案");
    }
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
//    e->acceptProposedAction(); //可以在这个窗口部件上拖放对象
//    if(e->mimeData()->hasFormat("text/uri-list")) //只能打开文本文件
//        e->acceptProposedAction(); //可以在这个窗口部件上拖放对象
}
void MainWindow::dropEvent(QDropEvent *event)
{
    //    QMessageBox::about(NULL, "提示", "dropEvent");
}

