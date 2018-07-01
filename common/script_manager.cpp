#include "script_manager.h"

#include <QFile>
#include <QDebug>
#include <QJSValue>
#include <QJSValueList>
#include <QJsonDocument>
#include <QJsonObject>
#include "utils.h"

ScriptManager::ScriptManager():console(NULL)
{

}

ScriptManager::init(QObject * console)
{
    this->console = console;
}

QJsonDocument ScriptManager::filter(const QString& projectName,const QJsonDocument& json,bool* isOk)
{
    qDebug() <<"filter filter input:"<< QString(json.toJson());
    QJSEngine engine;
    engine.installExtensions(QJSEngine::ConsoleExtension);
//    QJSValue consoleJs = engine.newQObject(console);
//    engine.globalObject().setProperty("console", consoleJs);

    QFileInfoList fileInfoList =  utils::listFileInfoList("data/plugins/filter",QDir::Files);
    foreach (QFileInfo fileInfo, fileInfoList) {
        QString jsStr = utils::readFileToQString(fileInfo.filePath());
        engine.evaluate(jsStr);
    }
    QDir projectPathDir("data/projects/"+projectName);
    QString filterJs = projectPathDir.filePath("filter.js");
    QString filterJsStr = utils::readFileToQString(filterJs);
//    QString filterJsWrap = "function __filterWrap(objstr){console.log('filterwrap');return JSON.stringify({\"myname\":213});}";
    QString filterJsWrap = "function __filterWrap(objstr){"
            "var __obj = JSON.parse(objstr);"
            "var res = filter(__obj);"
            "var resStr = JSON.stringify(res);"
            "console.log('filter res js:'+resStr);"
            "return resStr"
        "};\r\n"+filterJsStr;

    engine.evaluate(filterJsWrap);
    QJSValue filter = engine.globalObject().property("__filterWrap");
    QJSValueList args;
    args.append(QString(json.toJson()));

    QJSValue res = filter.call(args);
    if(isOk!=NULL){
        *isOk = !(res.isError());
    }
    if(res.isError()){
        QJsonObject jsonObj;
        jsonObj.insert("message",res.toString());
        QJsonDocument resJson(jsonObj);
        return resJson;
    }else{
        qDebug() <<"filter filter fn:"<< filter.isCallable();
        QJsonParseError error;
        QJsonDocument resJson = QJsonDocument::fromJson(res.toString().toUtf8(),&error);
        if(error.error != QJsonParseError::NoError){
            if(isOk!=NULL){
                *isOk = false;
            }
            QJsonObject jsonObj;
            jsonObj.insert("message",error.errorString());
            QJsonDocument resJson(jsonObj);
            return resJson;
        }else{
            qDebug() <<"filter res qtdoc:"<< QString(resJson.toJson());
            return resJson;
        }
    }
}


