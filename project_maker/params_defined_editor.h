#ifndef PARAMS_DEFINED_EDITOR_H
#define PARAMS_DEFINED_EDITOR_H

#include <QWidget>
#include <QString>
#include <QJsonObject>
#include <QEvent>
#include "project_maker/ieditor.h"

namespace Ui {
class ParamsDefinedEditor;
}

class ParamsDefinedEditor : public QWidget,public IEditor
{
    Q_OBJECT

public:
    explicit ParamsDefinedEditor(const QString& filePath,QWidget *parent = 0);
    virtual ~ParamsDefinedEditor();

    virtual bool editorClose();
    virtual QString editorFilePath();
    virtual bool editorSave();

    void appendRowData(QJsonObject jsonObj);
    void setRowData(int row,QJsonObject jsonObj);

    void deleteRowData(int row);

    void dragDrop(int fromRow,int toRow);

    bool eventFilter(QObject *obj, QEvent *eve);

private:
    Ui::ParamsDefinedEditor *ui;
    QString filePath;
    bool dirty;

};

#endif // PARAMS_DEFINED_EDITOR_H
