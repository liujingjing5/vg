#ifndef TEMPLATE_MANAGER_H
#define TEMPLATE_MANAGER_H

#include <QJSEngine>
#include <QJsonDocument>

class TemplateManager
{
public:
    TemplateManager();
    init(QObject * console);
    QString renderText(const QString& fmt,const QJsonDocument& data,bool* isOk=NULL);

private:
    QJSEngine* engine;
};

#endif // TEMPLATE_MANAGER_H
