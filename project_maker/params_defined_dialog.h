#ifndef PARAMS_DEFINED_DIALOG_H
#define PARAMS_DEFINED_DIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QJsonObject>
#include "params_defined_editor.h"

namespace Ui {
class ParamsDefinedDialog;
}

class ParamsDefinedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamsDefinedDialog(QWidget *parent = 0);
    explicit ParamsDefinedDialog(int row,const QJsonObject& jsonData,QWidget *parent = 0);
    ~ParamsDefinedDialog();

    void saveData();

private:
    void init();
    void typeChange();

    Ui::ParamsDefinedDialog *ui;
    ParamsDefinedEditor* paramsDefinedEditor;
    int row;
    QJsonObject jsonData;
    bool isEdit;
};

#endif // PARAMS_DEFINED_DIALOG_H
