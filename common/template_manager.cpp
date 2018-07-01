#include "template_manager.h"
#include <QFile>
#include <QDebug>
#include <QJSValue>
#include <QJSValueList>

TemplateManager::TemplateManager():engine(NULL)
{

}

TemplateManager::init(QObject *console)
{
    engine = new QJSEngine();
    engine->installExtensions(QJSEngine::ConsoleExtension);
    QFile nunjucksJsfile(":/asserts/js/nunjucks.js");
    qDebug() << "nunjuckJsfile:" << nunjucksJsfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString nunjucksJs = QString::fromUtf8(nunjucksJsfile.readAll());
//    qDebug() << "nunjucksJs:" << nunjucksJs;
//    engine.evaluate("");
//    QJSValue consoleJs = engine->newQObject(console);
//    engine->globalObject().setProperty("console", consoleJs);

    engine->evaluate(nunjucksJs);

}

QString TemplateManager::renderText(const QString& fmt, const QJsonDocument& data,bool* isOk)
{
    QJSValue global = engine->globalObject();
    QJSValue renderTextFn = global.property("renderText");
    QJSValueList args;
    args.append(fmt);
    args.append(QString(data.toJson()));
    QJSValue res = renderTextFn.call(args);
    if(isOk!=NULL){
        *isOk = !(res.isError());
    }
    return res.toString();
}
