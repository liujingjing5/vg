#ifndef PROJECT_MAKER_WINDOW_H
#define PROJECT_MAKER_WINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QToolBar>
#include <QPoint>
#include <QCloseEvent>

namespace Ui {
class ProjectMakerWindow;
}

class ProjectMakerWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct TreeSourceCodeDataEnum{
    //    const static int isLeaf = 1000;
        const static int path = 1001;
        const static int type = 1002;
    };

    struct TreeSourceCodeDataTypeEnum{
        const static int unkown = 0;
        const static int root = 1;
        const static int dir = 2;
        const static int file = 3;
        const static int tempalte = 4;
        const static int uijs = 5;
        const static int filterjs = 6;
        const static int readme = 7;
    };


    explicit ProjectMakerWindow(QString prjName,QWidget *parent = 0);
    ~ProjectMakerWindow();

    void refreshTreeSource();

    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *event);

    void handleTabCodeClose(int index);
    void handleTreeDoubleClicked(QTreeWidgetItem *item, int column);
    void handleTreeRightClicked(QPoint point);

    void treeMenuAddDir(QTreeWidgetItem* item);
    void treeMenuAddFile(QTreeWidgetItem* item);
    void treeMenuDelete(QTreeWidgetItem* item=NULL);
    void treeMenuRename(QTreeWidgetItem* item);
    void treeMenuRefresh(QTreeWidgetItem* item=NULL);

    void refreshTreeParams();


    void codeEditorInsertSymbol(int codeEditorInsertSymbol);


    void handleFinderFind();
    void handleFinderFindAndReplace();
    void handleFinderReplace();
    void handleFinderReplaceAll();

private:
    Ui::ProjectMakerWindow *ui;
    QString prjName;
    QMenu* treeSourceCodeMenu;
    QMenu* tabEditCodeMenu;
    QToolBar* toolBarDock;
};

#endif // PROJECT_MAKER_WINDOW_H
