#include "project_maker_window.h"
#include "ui_project_maker_window.h"


#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QContextMenuEvent>
#include "common/utils.h"
#include "common/rotationtoolbutton.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QMimeData>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "common/sg.h"
#include "common/utils.h"
#include "common/script_manager.h"
#include "common/file_type/file_type_utils.h"

#include "mainwindow.h"

#include "project_maker/code_editor.h"



void _collectFileDir(QTreeWidget* treeWidget,QTreeWidgetItem * parentWidgetItem, const QFileInfo & parent)
{
    QDir dir;
    dir.setPath(parent.filePath());
    dir.setFilter(QDir:: Files | QDir:: Dirs | QDir:: NoSymLinks);
    dir.setSorting(QDir:: DirsFirst | QDir:: Name);
    const QFileInfoList fileList = dir.entryInfoList();
    for (int i = 0; i < fileList.size(); i++) {
        QFileInfo fileInfo = fileList.at(i);
        QStringList fileColumn;
        fileColumn.append(fileInfo.fileName());
        if (fileInfo.fileName() == "." || fileInfo.fileName() == ".."){

        }else if (fileInfo.isDir()) {
            QTreeWidgetItem * child = new QTreeWidgetItem(fileColumn);
            child -> setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirIcon));
            child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::type,QVariant(ProjectMakerWindow::TreeSourceCodeDataTypeEnum::dir));
            child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::path,QVariant(fileInfo.fileName()));
            parentWidgetItem -> addChild(child);
            // 查询子目录
            _collectFileDir(treeWidget,child, fileInfo);
        }else if(fileInfo.isFile()){
            fileColumn.append(QString:: number(fileInfo.size()));
            QTreeWidgetItem * child = new QTreeWidgetItem(fileColumn);
            child -> setIcon(0, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
//            child->setData(0,TreeSourceCodeDataEnum::isLeaf,QVariant(true));
            child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::type,QVariant(ProjectMakerWindow::TreeSourceCodeDataTypeEnum::file));
            child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::path,QVariant(fileInfo.fileName()));
            parentWidgetItem -> addChild(child);
        }
    }
}

void _collectFile(QTreeWidget* treeWidget,QTreeWidgetItem * parentWidgetItem, const QFileInfo & fileInfo){
    if(fileInfo.isFile()){
        QTreeWidgetItem * child = new QTreeWidgetItem(parentWidgetItem);
        child->setText(0,fileInfo.fileName());
        child -> setIcon(0, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
//        child->setData(0,TreeSourceCodeDataEnum::isLeaf,QVariant(true));
        child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::type,QVariant(ProjectMakerWindow::TreeSourceCodeDataTypeEnum::file));
        child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::path,QVariant(fileInfo.fileName()));
        parentWidgetItem -> addChild(child);
    }else if(fileInfo.isDir()) {
        QTreeWidgetItem * child = new QTreeWidgetItem(parentWidgetItem);
        child->setText(0,fileInfo.fileName());
        child -> setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirIcon));
        child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::type,QVariant(ProjectMakerWindow::TreeSourceCodeDataTypeEnum::dir));
        child->setData(0,ProjectMakerWindow::TreeSourceCodeDataEnum::path,QVariant(fileInfo.fileName()));
        parentWidgetItem -> addChild(child);
        // 查询子目录
        _collectFileDir(treeWidget,child, fileInfo);
    }
}

QString _getPath(QTreeWidgetItem * widgetItem){
    QStringList list;
    while(widgetItem!=NULL){
        QString node = widgetItem->data(0,ProjectMakerWindow::TreeSourceCodeDataEnum::path).toString();
        list.append(node);
        widgetItem = widgetItem->parent();
    }
    QString path;
    for(int i = list.length()-1;i>=0;i--){
        if(!path.isEmpty()){
            path.append("/");
        }
        path.append(list.at(i));
    }
    return path;
}

void _clearQTreeWidgetItem(QTreeWidgetItem * widgetItem){
    int len = widgetItem->childCount();
    for(int i=0;i<len;i++){
        delete widgetItem->child(0);
    }
}




ProjectMakerWindow::ProjectMakerWindow(QString _prjName,QWidget *parent) :
    prjName(_prjName),QMainWindow(parent),
    ui(new Ui::ProjectMakerWindow)
{
    ui->setupUi(this);


    ui->toolBarCommon->setIconSize(QSize(16, 16));
    ui->toolBarCommon->addAction(QIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton)),"保存",[=]{
        IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->currentWidget());
        if(editor!=NULL){
            editor->editorSave();
        }
    });

    ui->toolBarEditorCode->setIconSize(QSize(16, 16));
    ui->toolBarEditorCode->addAction("var",[=]{
        this->codeEditorInsertSymbol(CodeEditor::TemplateSymbol::VAR);
    });
    ui->toolBarEditorCode->addAction("if",[=]{
        this->codeEditorInsertSymbol(CodeEditor::TemplateSymbol::IF);
    });
    ui->toolBarEditorCode->addAction("else",[=]{
        this->codeEditorInsertSymbol(CodeEditor::TemplateSymbol::ELSE);
    });
    ui->toolBarEditorCode->addAction("elseif",[=]{
        this->codeEditorInsertSymbol(CodeEditor::TemplateSymbol::ELSEIF);
    });
    ui->toolBarEditorCode->addAction("for",[=]{
        this->codeEditorInsertSymbol(CodeEditor::TemplateSymbol::FOR);
    });
//    QLineEdit* toolbarFindEdit = new QLineEdit(this);
//    toolbarFindEdit->setMaximumWidth(50);
//    toolbarFindEdit->setObjectName("toolbarFindEdit");
//    ui->toolBarEditorCode->addWidget(toolbarFindEdit);

//toolBarDock////////////////////////////////////
    toolBarDock = new QToolBar("窗口缩放栏", this);
    toolBarDock->setObjectName("dockBar");
    toolBarDock->setIconSize(QSize(16, 16));
    toolBarDock->setAllowedAreas(Qt::LeftToolBarArea);
    toolBarDock->setFloatable(false);
    toolBarDock->setMovable(false);
    addToolBar(Qt::LeftToolBarArea, toolBarDock);

    RotationToolButton *rtbtn = new RotationToolButton(toolBarDock);
    QAction* projectToggerAction = new QAction("模版方案", rtbtn);
    projectToggerAction->setCheckable(true);
    rtbtn->setDefaultAction(projectToggerAction);
    rtbtn->setRotation(RotationToolButton::CounterClockwise);
    rtbtn->setAutoRaise(true);
    toolBarDock->addWidget(rtbtn);

    connect(projectToggerAction,&QAction::triggered,[=](bool checked){
        ui->dockProject->setVisible(checked);
    });
    connect(ui->dockProject,&QDockWidget::visibilityChanged,[=](bool checked){
        projectToggerAction->setChecked(checked);
    });

    RotationToolButton *codeToolbtn = new RotationToolButton(toolBarDock);
    QAction* codeToolToggerAction = new QAction("代码工具", codeToolbtn);
    codeToolToggerAction->setCheckable(true);
    codeToolbtn->setDefaultAction(codeToolToggerAction);
    codeToolbtn->setRotation(RotationToolButton::CounterClockwise);
    codeToolbtn->setAutoRaise(true);
    toolBarDock->addWidget(codeToolbtn);

    connect(codeToolToggerAction,&QAction::triggered,[=](bool checked){
        ui->dockCodeTool->setVisible(checked);
    });
    connect(ui->dockCodeTool,&QDockWidget::visibilityChanged,[=](bool checked){
        codeToolToggerAction->setChecked(checked);
    });


    RotationToolButton *paramsbtn = new RotationToolButton(toolBarDock);
    QAction* paramsbtnToggerAction = new QAction("参数列表", paramsbtn);
    paramsbtnToggerAction->setCheckable(true);
    paramsbtn->setDefaultAction(paramsbtnToggerAction);
    paramsbtn->setRotation(RotationToolButton::CounterClockwise);
    paramsbtn->setAutoRaise(true);
    toolBarDock->addWidget(paramsbtn);

    connect(paramsbtnToggerAction,&QAction::triggered,[=](bool checked){
        ui->dockParams->setVisible(checked);
    });
    connect(ui->dockParams,&QDockWidget::visibilityChanged,[=](bool checked){
        paramsbtnToggerAction->setChecked(checked);
    });





    connect(ui->btnFinderFind,&QPushButton::clicked,[=](bool checked){
        handleFinderFind();
    });
    connect(ui->btnFinderReplace,&QPushButton::clicked,[=](bool checked){
        handleFinderReplace();
    });
    connect(ui->btnFinderFindAndReplace,&QPushButton::clicked,[=](bool checked){
        handleFinderFindAndReplace();
    });
    connect(ui->btnFinderReplaceAll,&QPushButton::clicked,[=](bool checked){
        handleFinderReplaceAll();
    });


//tabEditCode////////////////////////////////////////
    tabEditCodeMenu = new QMenu(this);
    QAction* tabEditCodeMenuCloseAction = new QAction("关闭", tabEditCodeMenu);
    QAction* tabEditCodeMenuCloseOtherAction = new QAction("关闭其他", tabEditCodeMenu);
    QAction* tabEditCodeMenuCloseAllAction = new QAction("关闭所有", tabEditCodeMenu);
    tabEditCodeMenu->addAction(tabEditCodeMenuCloseAction);
    tabEditCodeMenu->addAction(tabEditCodeMenuCloseOtherAction);
    tabEditCodeMenu->addAction(tabEditCodeMenuCloseAllAction);

    connect(tabEditCodeMenuCloseAction,&QAction::triggered,this,[=](bool checked){
        QVariant indexVar = tabEditCodeMenu->property("index");
        if(!indexVar.isNull()){
            int index = indexVar.toInt();
            IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->widget(index));
            if(editor->editorClose()){
                ui->tabEditCode->removeTab(index);
            }else{
                return;
            }
        }
    });
    connect(tabEditCodeMenuCloseOtherAction,&QAction::triggered,this,[=](bool checked){
        QVariant indexVar = tabEditCodeMenu->property("index");
        if(!indexVar.isNull()){
            int index = indexVar.toInt();
            int tablen = ui->tabEditCode->count();
            for(int i=0;i< tablen;i++){
                int removeIndex=0;
                if(index==i){
                    continue;
                }else if(index<i){
                    removeIndex=1;
                }
                IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->widget(removeIndex));
                if(editor->editorClose()){
                    ui->tabEditCode->removeTab(removeIndex);
                }else{
                    return;
                }
            }
        }
    });
    connect(tabEditCodeMenuCloseAllAction,&QAction::triggered,this,[=](bool checked){
        int tablen = ui->tabEditCode->count();
        for(int i=0;i< tablen;i++){
            IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->widget(0));
            if(editor->editorClose()){
                ui->tabEditCode->removeTab(0);
            }else{
                return;
            }
        }
    });

    connect(ui->tabEditCode,&QTabWidget::tabCloseRequested,this,&ProjectMakerWindow::handleTabCodeClose);
    connect(ui->tabEditCode,&QTabWidget::tabBarDoubleClicked,this,&ProjectMakerWindow::handleTabCodeClose);
    connect(ui->tabEditCode,&QTabWidget::currentChanged,this,[=]{
        QWidget* widget = ui->tabEditCode->currentWidget();
        if(widget!=NULL){
            int type = widget->property("fileType").toInt();
            if(type==TreeSourceCodeDataTypeEnum::file){
                ui->toolBarEditorCode->setEnabled(true);
            }else{
                ui->toolBarEditorCode->setEnabled(false);
            }
        }
    });
    ui->tabEditCode->installEventFilter(this);


//treeSourceFile///////////////////////////////////////
    treeSourceCodeMenu = new QMenu(this);
    ui->treeSourceFile->setDragEnabled(true);
    ui->treeSourceFile->setAcceptDrops(true);
//    ui->treeSourceFile->setDropIndicatorShown(true);
    ui->treeSourceFile->setDragDropMode(QAbstractItemView::InternalMove);
    ui->treeSourceFile->setContextMenuPolicy(Qt::CustomContextMenu);//自己拦截事件处理了
    ui->treeSourceFile->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(ui->treeSourceFile,&QTreeWidget::itemDoubleClicked,this,&ProjectMakerWindow::handleTreeDoubleClicked);
    connect(ui->treeSourceFile,&QTreeWidget::customContextMenuRequested,this,&ProjectMakerWindow::handleTreeRightClicked);

    ui->treeSourceFile->installEventFilter(this);             //按钮
    ui->treeSourceFile->viewport()->installEventFilter(this); //拖拽
    ui->treeSourceFile->header()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //treeParams///////////////////////////////////////


    ui->treeParams->setContextMenuPolicy(Qt::CustomContextMenu);//自己拦截事件处理了
    ui->treeParams->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->treeParams->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->treeParams,&QTreeWidget::itemDoubleClicked,this,[=](QTreeWidgetItem *item, int column){
        if(item->parent()==NULL){
            refreshTreeParams();
        }
    });

    QTimer::singleShot(100, this,[=]{
        this->refreshTreeSource();
        this->refreshTreeParams();
//        emit ui->dockProject->visibilityChanged(ui->dockProject->isVisible());
    });


}


ProjectMakerWindow::~ProjectMakerWindow()
{
    delete ui;
}

void ProjectMakerWindow::refreshTreeSource()
{
    ui->treeSourceFile->clear();
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeSourceFile,QStringList(prjName));
    rootItem->setIcon(0,ui->treeSourceFile->style()->standardIcon(QStyle::SP_DirHomeIcon));
    rootItem->setData(0,TreeSourceCodeDataEnum::type,QVariant(TreeSourceCodeDataTypeEnum::root));
    rootItem->setData(0,TreeSourceCodeDataEnum::path,QVariant(QString("data/projects/"+prjName)));

    QTreeWidgetItem *uiItem = new QTreeWidgetItem(rootItem,QStringList("参数输入界面"));
    uiItem ->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
//    uiItem->setData(0,TreeSourceCodeDataEnum::isLeaf,QVariant(true));
    uiItem->setData(0,TreeSourceCodeDataEnum::type,QVariant(TreeSourceCodeDataTypeEnum::uijs));
    uiItem->setData(0,TreeSourceCodeDataEnum::path,QVariant(QString("ui.json")));

    QTreeWidgetItem *filterItem = new QTreeWidgetItem(rootItem,QStringList("过滤器脚本"));
    filterItem->setIcon(0,ui->treeSourceFile->style()->standardIcon(QStyle::SP_FileIcon));
//    filterItem->setData(0,TreeSourceCodeDataEnum::isLeaf,QVariant(true));
    filterItem->setData(0,TreeSourceCodeDataEnum::type,QVariant(TreeSourceCodeDataTypeEnum::filterjs));
    filterItem->setData(0,TreeSourceCodeDataEnum::path,QVariant(QString("filter.js")));

    QTreeWidgetItem *readmeItem = new QTreeWidgetItem(rootItem,QStringList("方案介绍"));
    readmeItem->setIcon(0,ui->treeSourceFile->style()->standardIcon(QStyle::SP_FileIcon));
//    readmeItem->setData(0,TreeSourceCodeDataEnum::isLeaf,QVariant(true));
    readmeItem->setData(0,TreeSourceCodeDataEnum::type,QVariant(TreeSourceCodeDataTypeEnum::readme));
    readmeItem->setData(0,TreeSourceCodeDataEnum::path,QVariant(QString("readme.md")));

    QTreeWidgetItem *templateItem = new QTreeWidgetItem(rootItem,QStringList("模版文件"));
    templateItem->setIcon(0,ui->treeSourceFile->style()->standardIcon(QStyle::SP_DirIcon));
    templateItem->setData(0,TreeSourceCodeDataEnum::type,QVariant(TreeSourceCodeDataTypeEnum::tempalte));
    templateItem->setData(0,TreeSourceCodeDataEnum::path,QVariant(QString("template")));
    _collectFileDir(ui->treeSourceFile,templateItem,QFileInfo(QString("data/projects/")+prjName+"/template"));
//    ui->treeSourceFile->installEventFilter(this);
    rootItem->setExpanded(true);
    templateItem->setExpanded(true);


}

bool ProjectMakerWindow::eventFilter(QObject *obj, QEvent *event)
{
//    qDebug() << "event  "<<event  ;
    if (obj == ui->treeSourceFile->viewport()) {
//        qDebug() << event ;
        if (event->type() == QEvent::DragEnter) {
            QDragEnterEvent *e = static_cast<QDragEnterEvent *>(event);
            QTreeWidgetItem *item = ui->treeSourceFile->itemAt(e->pos());
            if(item!=NULL){
    //            int treeItemType = item->data(0,TreeSourceCodeDataEnum::type).toInt();
    //            qDebug() << "obj:  "<<item->text(0)  ;
    //            if(treeItemType==TreeSourceCodeDataTypeEnum::dir || treeItemType==TreeSourceCodeDataTypeEnum::tempalte ){
    //            }
            }
            e->acceptProposedAction();
            return false;
        }else if (event->type() == QEvent::DragMove) {
            QDragMoveEvent *e = static_cast<QDragMoveEvent *>(event);
            bool isInner =  e->mimeData()->urls().isEmpty();
            QTreeWidgetItem *item = ui->treeSourceFile->itemAt(e->pos());
            if(item!=NULL){
                int treeItemType = item->data(0,TreeSourceCodeDataEnum::type).toInt();
                if(treeItemType==TreeSourceCodeDataTypeEnum::dir || treeItemType==TreeSourceCodeDataTypeEnum::tempalte){
                    QTreeWidgetItem * startItem = ui->treeSourceFile->currentItem();
                    if(isInner){
                        if(startItem == item){
                            e->ignore();
                            return true;
                        }else{
                            QList<QTreeWidgetItem *> selectItems = ui->treeSourceFile->selectedItems();
                            bool sameLevel = true;
                            QTreeWidgetItem *itemParent = NULL;
                            foreach (QTreeWidgetItem * selectItem, selectItems) {
                                int type = selectItem->data(0,TreeSourceCodeDataEnum::type).toInt();
                                if(type== TreeSourceCodeDataTypeEnum::dir || type== TreeSourceCodeDataTypeEnum::file ){
                                    if(itemParent==NULL){
                                        itemParent = selectItem->parent();
                                    }else if(itemParent != selectItem->parent()){
                                        sameLevel=false;
                                        break;
                                    }
                                }else{
                                    e->ignore();
                                    return true;
                                }
                            }
                            if(sameLevel){
                                e->acceptProposedAction();
                                return true;
                            }else{
                                e->ignore();
                                return true;
                            };
                        }
                    }else{
                        e->acceptProposedAction();
                        return true;
                    }
                }else{
                    e->ignore();
                    return true;
                }
            }
        }else if(event->type() == QEvent::Drop){
            QDropEvent *e = static_cast<QDropEvent *>(event);
            bool isInner =  e->mimeData()->urls().isEmpty();
            const QMimeData *mimeData = e->mimeData();//获取mimie数据
            QList<QUrl> urlList =  mimeData->urls();
            QTreeWidgetItem * startItem = ui->treeSourceFile->currentItem();
            QTreeWidgetItem * endItem = ui->treeSourceFile->itemAt(e->pos());
            QString startPath = _getPath(startItem);;
            QString endDirPath = _getPath(endItem);

            if(isInner){ //内部拖拽
               if(startItem!=NULL){
                   if(startItem!=endItem){
                       if (e->dropAction() == Qt::MoveAction){
                           QList<QTreeWidgetItem *> selectItems = ui->treeSourceFile->selectedItems();
                           foreach (QTreeWidgetItem * selectItem, selectItems) {
                               QString selectPath = _getPath(selectItem);
                               qDebug() << "mv startPath:" << selectPath<<" endPath:"<< (endDirPath+"/"+QFileInfo(selectPath).fileName());
                               if(utils::mv(selectPath,endDirPath+"/"+QFileInfo(selectPath).fileName())){
                                   delete selectItem;
//                                   QTreeWidgetItem * selectParentItem = selectItem->parent();
//                                   selectParentItem->removeChild(selectItem);
//                                   endItem->addChild(selectItem);
                               };
                           }
                       }else if(e->dropAction() == Qt::CopyAction){
                           QList<QTreeWidgetItem *> selectItems = ui->treeSourceFile->selectedItems();
                           foreach (QTreeWidgetItem * selectItem, selectItems) {
                               QString selectPath = _getPath(selectItem);
                               qDebug() << "copy startPath:" << selectPath<<" endPath:"<< (endDirPath+"/"+QFileInfo(selectPath).fileName());
                               if(utils::copy(selectPath,endDirPath+"/"+QFileInfo(selectPath).fileName(),true)){
//                                   QTreeWidgetItem * newItem = selectItem->clone();
//                                   endItem->addChild(newItem);
                               };
                           }
                       }
                   }
               }
            }else{
                foreach (QUrl url, urlList) {
                    if(url.isLocalFile()){
                        QFileInfo sourceFileInfo(url.toLocalFile());
                        QString targetPath = endDirPath+"/"+sourceFileInfo.fileName();
                        utils::copy(sourceFileInfo.filePath(),targetPath,true);
                        qDebug() << url.toLocalFile();
                    }
                }
            }
            _clearQTreeWidgetItem(endItem);
            _collectFileDir(ui->treeSourceFile,endItem,QFileInfo(_getPath(endItem)));
            return true;
        }
    }else if(obj == ui->treeSourceFile){
        if(event->type() == QEvent::KeyRelease){
            QKeyEvent * e = static_cast<QKeyEvent  *>(event) ;
            if(e->key() == Qt::Key_Delete){
                treeMenuDelete();
            }else if(e->key() == Qt::Key_F2){
                QTreeWidgetItem* item =  ui->treeSourceFile->currentItem();
                if(item!=NULL){
                    int type = item->data(0,TreeSourceCodeDataEnum::type).toInt();
                    if(type== TreeSourceCodeDataTypeEnum::dir ||type== TreeSourceCodeDataTypeEnum::file ){
                        treeMenuRename(ui->treeSourceFile->currentItem());
                    }
                }
            }else if(e->key() == Qt::Key_F5){
                treeMenuRefresh();
            }
        }
    }else if(obj == ui->tabEditCode){
        if(event->type() == QEvent::MouseButtonRelease){
            QMouseEvent * e = static_cast<QMouseEvent  *>(event) ;
            if(e->button()==Qt::RightButton){
                QPoint p = e->pos();
//                qDebug() << "RightButton:" << p;
                int index =  ui->tabEditCode->tabBar()->tabAt(p);
                if(index>=0){
                    tabEditCodeMenu->setProperty("index",index);
                    e->ignore();
                    tabEditCodeMenu->exec(QCursor::pos());
                }
                return true;
            }else if(e->button()==Qt::MiddleButton){
                QPoint p = e->pos();
                int index =  ui->tabEditCode->tabBar()->tabAt(p);
                if(index>=0){
                    e->ignore();
                    IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->widget(index));
                    if(editor->editorClose()){
                        ui->tabEditCode->removeTab(index);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

void ProjectMakerWindow::closeEvent(QCloseEvent *event)
{
    int tablen = ui->tabEditCode->count();
    for(int i=0;i< tablen;i++){
        IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->widget(0));
        if(editor->editorClose()){
            ui->tabEditCode->removeTab(0);
        }else{
            event->ignore();
            return;
        }
    }
    ((MainWindow*)this->parent())->refreshProjectParams();
}

void ProjectMakerWindow::handleTreeDoubleClicked(QTreeWidgetItem *item, int column)
{
    int type = item->data(0,TreeSourceCodeDataEnum::type).toInt();
    if(type==TreeSourceCodeDataTypeEnum::file || type==TreeSourceCodeDataTypeEnum::filterjs || type==TreeSourceCodeDataTypeEnum::uijs  || type==TreeSourceCodeDataTypeEnum::readme  ){
        QString filepath = _getPath(item);
        QFileInfo fileInfo(filepath);
        qDebug() << "handleTreeDoubleClicked:"<<filepath;
        bool tabExist =false;
        int tablen = ui->tabEditCode->count();
        for(int i=0;i< tablen;i++){
            IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->widget(i));
            if(editor->editorFilePath() == filepath){
                ui->tabEditCode->setCurrentIndex(i);
                tabExist=true;
                break;
            }
        }
        if(!tabExist){
            int oldCurrIndex = ui->tabEditCode->currentIndex();
            FileTypeUtils::FileType fileType = FileTypeUtils::findFileTypeByFilePath(filepath);
            if(fileType.type!="text"){
                QMessageBox::warning(NULL,"警告","非文本文件，不可以编辑");
                return;
            }
            CodeEditor *editor=new CodeEditor(filepath,fileType.characterSet,this);
            editor->setProperty("fileType",type);
            ui->tabEditCode->insertTab(oldCurrIndex+1,editor,QApplication::style()->standardIcon(QStyle::SP_DialogYesButton),fileInfo.fileName());
            ui->tabEditCode->setCurrentIndex(oldCurrIndex+1);
            connect(editor,&CodeEditor::modificationChanged,this,[=](bool changed){
                qDebug() << "changeddddd" << changed;
                int atcindex = ui->tabEditCode->indexOf(editor);
                if(atcindex!=-1){
                    if(changed){
                        ui->tabEditCode->setTabIcon(atcindex,QApplication::style()->standardIcon(QStyle::SP_DialogNoButton));
                    }else{
                        ui->tabEditCode->setTabIcon(atcindex,QApplication::style()->standardIcon(QStyle::SP_DialogYesButton));
                    }
                }
            });



        }
    }
}

void ProjectMakerWindow::treeMenuAddDir(QTreeWidgetItem* item){
    if(item!=NULL){
        int type = item->data(0,TreeSourceCodeDataEnum::type).toInt();
        if(type== TreeSourceCodeDataTypeEnum::dir ||type== TreeSourceCodeDataTypeEnum::tempalte ){
            bool isOK;
            QString name = QInputDialog::getText(this, "重命名","请输入名称",QLineEdit::Normal, "", &isOK).trimmed();
            if(isOK) {
                if(name.isEmpty()){
                    QMessageBox::warning(this, "警告","新名称不能为空");
                    return;
                }
            }else{
                return;
            }
            QString dirPath = _getPath(item);
            QString newDirPath = dirPath+"/"+name;
            if(QFile::exists(newDirPath)){
                QMessageBox::warning(this, "警告","已存在");
                return;
            }
            QDir(newDirPath).mkdir(".");
            QTreeWidgetItem * child = new QTreeWidgetItem(item);
            child->setText(0,name);
            child ->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirIcon));
        //    child->setData(0,TreeSourceCodeDataEnum::isLeaf,QVariant(false));
            child->setData(0,TreeSourceCodeDataEnum::type,QVariant(TreeSourceCodeDataTypeEnum::dir));
            child->setData(0,TreeSourceCodeDataEnum::path,QVariant(name));
            item ->addChild(child);
        }
    }
}

void ProjectMakerWindow::treeMenuAddFile(QTreeWidgetItem* item){
    if(item!=NULL){
        int type = item->data(0,TreeSourceCodeDataEnum::type).toInt();
        if(type== TreeSourceCodeDataTypeEnum::dir ||type== TreeSourceCodeDataTypeEnum::file ||type== TreeSourceCodeDataTypeEnum::tempalte ){
            bool isOK;
            QString name = QInputDialog::getText(this, "重命名","请输入名称",QLineEdit::Normal, "", &isOK).trimmed();
            if(isOK) {
                if(name.isEmpty()){
                    QMessageBox::warning(this, "警告","新名称不能为空");
                    return;
                }
            }else{
                return;
            }
            QString dirPath = _getPath(item);
            QString newfilePath = dirPath+"/"+name;
            if(QFile::exists(newfilePath)){
                QMessageBox::warning(this, "警告","已存在");
                return;
            }
            utils::writeQStringToFile("",newfilePath);
            QTreeWidgetItem * child = new QTreeWidgetItem(item);
            child->setText(0,name);
            child ->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
        //    child->setData(0,TreeSourceCodeDataEnum::isLeaf,QVariant(true));
            child->setData(0,TreeSourceCodeDataEnum::type,QVariant(TreeSourceCodeDataTypeEnum::file));
            child->setData(0,TreeSourceCodeDataEnum::path,QVariant(name));
            item ->addChild(child);
        }
    }
}

void ProjectMakerWindow::treeMenuDelete(QTreeWidgetItem* item){
    QList<QTreeWidgetItem *> selectItems = ui->treeSourceFile->selectedItems();
    QList<QTreeWidgetItem *> selectItemsFiltered ;

    foreach (QTreeWidgetItem * selectItem, selectItems) {
        int type = selectItem->data(0,TreeSourceCodeDataEnum::type).toInt();
        if(type== TreeSourceCodeDataTypeEnum::dir ||type== TreeSourceCodeDataTypeEnum::file ){
            bool isChild=false;
            QTreeWidgetItem *currentItem = selectItem->parent();
            while(currentItem!=NULL){
                if(selectItems.contains(currentItem)){
                    isChild=true;
                    break;
                }
                currentItem = currentItem->parent();
            }
            if(!isChild){
                selectItemsFiltered.append(selectItem);
            }
        }
    }
    if(!selectItemsFiltered.isEmpty()){
        int res = QMessageBox::information(NULL,"消息","是否删除",QMessageBox::Yes|QMessageBox::No);
        if(res==QMessageBox::Yes){
            foreach (QTreeWidgetItem * selectItem, selectItemsFiltered) {
                QString filePath = _getPath(selectItem);
                if(utils::remove(filePath)){
                    qDebug() << "utils::remove";
                    delete selectItem;
                };
            }
        }
    }
}

void ProjectMakerWindow::treeMenuRename(QTreeWidgetItem* item){
    if(item!=NULL){
        int type = item->data(0,TreeSourceCodeDataEnum::type).toInt();
        if(type== TreeSourceCodeDataTypeEnum::dir ||type== TreeSourceCodeDataTypeEnum::file ){
            QString filePath = _getPath(item);
            bool isOK;
            QString name = QInputDialog::getText(this, "重命名","请输入新名称",QLineEdit::Normal, QFileInfo(filePath).fileName(), &isOK).trimmed();
            if(isOK) {
                if(name.isEmpty()){
                    QMessageBox::warning(this, "警告","方案名称不能为空");
                    return;
                }
            }else{
                return;
            }
            QString newfilePath = QFileInfo(filePath).dir().filePath(name);
            if(QFile::exists(newfilePath)){
                QMessageBox::warning(this, "警告","已存在");
                return;
            }
            if(utils::rename(filePath,name)){
                qDebug() << "utils::remove";
                item->setText(0,name);
                item->setData(0,TreeSourceCodeDataEnum::path,QVariant(name));
            };
        }
    }else{
        QMessageBox::critical(NULL, "错误", "请选择文件或文件夹");
    }
}

void ProjectMakerWindow::treeMenuRefresh(QTreeWidgetItem *item)
{
    this->refreshTreeSource();
}

void _collectParams(QTreeWidgetItem * parentWidgetItem, const QJsonObject & jsonObj){

    QJsonObject::const_iterator ite;
    for(ite=jsonObj.begin();ite!=jsonObj.end();ite++)
    {
        QString key = ite.key();
        QTreeWidgetItem * child = new QTreeWidgetItem(parentWidgetItem);
        child->setText(0,key);
//        child -> setIcon(0, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
        parentWidgetItem -> addChild(child);
        if(ite.value().isObject()){
            _collectParams(child,QJsonObject(ite.value().toObject()));
        }
    }

}

void ProjectMakerWindow::refreshTreeParams()
{
    ui->treeParams->clear();
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeParams,QStringList("刷新"));
    rootItem->setIcon(0,ui->treeSourceFile->style()->standardIcon(QStyle::SP_BrowserReload));
    rootItem->setExpanded(true);


    QString uipath("data/projects/"+prjName+"/ui.json");
    QString uiStr = utils::readFileToQString(uipath);
    QJsonObject newObj;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(uiStr.toUtf8(),&error);
    if(error.error != QJsonParseError::NoError){
        QMessageBox::critical(NULL,"参数输入界面定义错误", error.errorString());
        return;
    }
    foreach (QJsonValue jsonValue, doc.array()) {
        QJsonObject jsonObj = jsonValue.toObject();
        qDebug() <<"jsonObj:"<< jsonObj ;
        QString type = jsonObj.value("type").toString();
        QString id = jsonObj.value("id").toString();
        if(type=="checkbox"){
            QJsonArray value = jsonObj.value("value").toArray();
            newObj.insert(id,value);
        }else{
            QString value = jsonObj.value("value").toString();
            newObj.insert(id,value);
        }
    }
    QJsonDocument jsonDoc(newObj);
    bool isOk;
    QJsonDocument jsonDocPlus = sg<ScriptManager>::get()->filter(prjName,jsonDoc,&isOk);
    if(!isOk){
        QMessageBox::critical(NULL,"过滤器错误", jsonDocPlus.object().value("message").toString());
        return;
    }
    _collectParams(rootItem,jsonDocPlus.object());
    QString paramsStr(jsonDocPlus.toJson());
    qDebug() <<"paramsStr:"<< paramsStr;


}





void ProjectMakerWindow::handleTreeRightClicked(QPoint point)
{
    qDebug() << "QContextMenuEvent";
    treeSourceCodeMenu->clear();
    QTreeWidgetItem *item = ui->treeSourceFile->itemAt(point);
    if(item==NULL){
        return;
    }
    int nodeType = item->data(0,TreeSourceCodeDataEnum::type).toInt();
    if(nodeType==TreeSourceCodeDataTypeEnum::tempalte){
        QAction* m_addDirAction = new QAction("增加目录..", treeSourceCodeMenu);
        QAction* m_addFileAction = new QAction("增加文件..", treeSourceCodeMenu);
        QAction* m_localOpenAction = new QAction("资源管理器打开", treeSourceCodeMenu);
        treeSourceCodeMenu->addAction(m_addDirAction);
        treeSourceCodeMenu->addAction(m_addFileAction);
        treeSourceCodeMenu->addAction(m_localOpenAction);
        connect(m_addDirAction,&QAction::triggered,this,[=](bool checked){
            treeMenuAddDir(item);
        });
        connect(m_addFileAction,&QAction::triggered,this,[=](bool checked){
            treeMenuAddFile(item);
        });
        connect(m_localOpenAction,&QAction::triggered,this,[=](bool checked){
            QDesktopServices::openUrl(QUrl("file:///"+QDir::currentPath()+"/"+_getPath(item), QUrl::TolerantMode));
        });
        treeSourceCodeMenu->exec(QCursor::pos());
    }else if(nodeType==TreeSourceCodeDataTypeEnum::dir){
        QAction* m_addDirAction = new QAction("增加目录...", treeSourceCodeMenu);
        QAction* m_delAction = new QAction("删除...", treeSourceCodeMenu);
        QAction* m_addFileAction = new QAction("增加文件...", treeSourceCodeMenu);
        QAction* m_renameAction = new QAction("重命名...", treeSourceCodeMenu);
        QAction* m_localOpenAction = new QAction("资源管理器打开", treeSourceCodeMenu);
        treeSourceCodeMenu->addAction(m_addDirAction);
        treeSourceCodeMenu->addAction(m_delAction);
        treeSourceCodeMenu->addAction(m_addFileAction);
        treeSourceCodeMenu->addAction(m_renameAction);
        treeSourceCodeMenu->addAction(m_localOpenAction);
        connect(m_addDirAction,&QAction::triggered,this,[=](bool checked){
            treeMenuAddDir(item);
        });
        connect(m_addFileAction,&QAction::triggered,this,[=](bool checked){
            treeMenuAddFile(item);
        });
        connect(m_delAction,&QAction::triggered,this,[=](bool checked){
            treeMenuDelete(item);
        });
        connect(m_renameAction,&QAction::triggered,this,[=](bool checked){
            treeMenuRename(item);
        });
        connect(m_localOpenAction,&QAction::triggered,this,[=](bool checked){
            QDesktopServices::openUrl(QUrl("file:///"+QDir::currentPath()+"/"+_getPath(item), QUrl::TolerantMode));
        });
        treeSourceCodeMenu->exec(QCursor::pos());
    }else if(nodeType==TreeSourceCodeDataTypeEnum::file){
        QAction* m_addFileAction = new QAction("增加文件...", treeSourceCodeMenu);
        QAction* m_delAction = new QAction("删除...", treeSourceCodeMenu);
        QAction* m_renameAction = new QAction("重命名...", treeSourceCodeMenu);
        QAction* m_localOpenAction = new QAction("资源管理器打开", treeSourceCodeMenu);
        treeSourceCodeMenu->addAction(m_localOpenAction);
        treeSourceCodeMenu->addAction(m_addFileAction);
        treeSourceCodeMenu->addAction(m_delAction);
        treeSourceCodeMenu->addAction(m_renameAction);
        treeSourceCodeMenu->addAction(m_localOpenAction);
        connect(m_addFileAction,&QAction::triggered,this,[=](bool checked){
            treeMenuAddFile(item);
        });
        connect(m_delAction,&QAction::triggered,this,[=](bool checked){
            treeMenuDelete(item);
        });
        connect(m_renameAction,&QAction::triggered,this,[=](bool checked){
            treeMenuRename(item);
        });
        connect(m_localOpenAction,&QAction::triggered,this,[=](bool checked){
            QDesktopServices::openUrl(QUrl("file:///"+QDir::currentPath()+"/"+QFileInfo(_getPath(item)).dir().path(), QUrl::TolerantMode));
        });
        treeSourceCodeMenu->exec(QCursor::pos());
    }else if(nodeType==TreeSourceCodeDataTypeEnum::root){
        QAction* m_refreshAction = new QAction("刷新", treeSourceCodeMenu);
        QAction* m_localOpenAction = new QAction("资源管理器打开", treeSourceCodeMenu);
        treeSourceCodeMenu->addAction(m_refreshAction);
        treeSourceCodeMenu->addAction(m_localOpenAction);
        connect(m_refreshAction,&QAction::triggered,this,[=](bool checked){
            treeMenuRefresh(item);
        });
        connect(m_localOpenAction,&QAction::triggered,this,[=](bool checked){
            QDesktopServices::openUrl(QUrl("file:///"+QDir::currentPath()+"/"+_getPath(item), QUrl::TolerantMode));
        });
        treeSourceCodeMenu->exec(QCursor::pos());
    }
}


void ProjectMakerWindow::handleTabCodeClose(int index)
{

    IEditor* editor = dynamic_cast<IEditor*>(ui->tabEditCode->widget(index));
    if(editor->editorClose()){
        ui->tabEditCode->removeTab(index);
    }

}



void ProjectMakerWindow::codeEditorInsertSymbol(int codeEditorInsertSymbol)
{
    QWidget* widget = ui->tabEditCode->currentWidget();
    if(widget!=NULL){
        CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(widget);
        codeEditor->codeEditorInsertSymbol(codeEditorInsertSymbol);
    }
}

void ProjectMakerWindow::handleFinderFind()
{
    QWidget* widget = ui->tabEditCode->currentWidget();
    QString word = ui->editFinderFind->text();
    bool direction = ui->radioDirection->isChecked();
    bool caseSensitive = ui->cbxFinderCase->isChecked();
    bool reg = ui->radioFinderReg->isChecked() ;

    if(widget!=NULL){
        CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(widget);
        codeEditor->finderFind(word,reg,direction,caseSensitive);
        codeEditor->finderHighlight(word,reg,caseSensitive);
    }
}

void ProjectMakerWindow::handleFinderFindAndReplace()
{
    QWidget* widget = ui->tabEditCode->currentWidget();
    QString word = ui->editFinderFind->text();
    QString toWord = ui->editFinderReplace->text();
    bool direction = ui->radioDirection->isChecked();
    bool caseSensitive = ui->cbxFinderCase->isChecked();
    bool reg = ui->radioFinderReg->isChecked() ;
    if(widget!=NULL){
        CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(widget);
        codeEditor->finderReplace(2,word,toWord,reg,direction,caseSensitive);
        codeEditor->finderHighlight(word,reg,caseSensitive);
    }
}

void ProjectMakerWindow::handleFinderReplace()
{
    QWidget* widget = ui->tabEditCode->currentWidget();
    QString word = ui->editFinderFind->text();
    QString toWord = ui->editFinderReplace->text();
    bool direction = ui->radioDirection->isChecked();
    bool caseSensitive = ui->cbxFinderCase->isChecked();
    bool reg = ui->radioFinderReg->isChecked() ;
    if(widget!=NULL){
        CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(widget);
        codeEditor->finderReplace(1,word,toWord,reg,direction,caseSensitive);
        codeEditor->finderHighlight(word,reg,caseSensitive);
    }
}

void ProjectMakerWindow::handleFinderReplaceAll()
{
    QWidget* widget = ui->tabEditCode->currentWidget();
    QString word = ui->editFinderFind->text();
    QString toWord = ui->editFinderReplace->text();
    bool caseSensitive = ui->cbxFinderCase->isChecked();
    bool reg = ui->radioFinderReg->isChecked() ;

    if(widget!=NULL){
        CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(widget);
        int num = codeEditor->finderReplaceAll(word,toWord,reg,caseSensitive);
        QMessageBox::information(NULL,"消息","替换了"+QString::number(num)+"个");
    }
}


