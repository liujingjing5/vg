#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <QJSEngine>
#include <QJsonDocument>

class ScriptManager
{
public:
    ScriptManager();
    init(QObject * console);
    QJsonDocument filter(const QString& projectName,const QJsonDocument& json,bool* isOk=NULL);
private:
    QObject *console;
};

#endif // SCRIPT_MANAGER_H
