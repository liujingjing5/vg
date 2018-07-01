#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QDropEvent>
#include <QDragEnterEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void refreshCbxProject(const QString& oldPrjName = QString() );

    void refreshProjectParams();
    QJsonObject getProjectParamsValues(bool needRequire,bool* isOk);
    void setProjectParamsValues(const QJsonObject&  jsonObj);

    void handleBtnTargetPath();
    void handleCbxProjectChange(int index);
    void handleBtnCodeGen(bool show);

    void handleProjectCreate();
    void handleProjectModify();
    void handleProjectDelete();
    void handleProjectInfo();

    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *e);
    virtual void dropEvent(QDropEvent *event);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
