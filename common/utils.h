#ifndef UTILS_H
#define UTILS_H

#include <QFileInfo>
#include <QString>
#include <QDir>
#include <QFile>
#include "common/biz_error.h"

namespace utils {

    QFileInfoList listFileInfoList(QString dirpath,QDir::Filters findFlag=QDir::Files | QDir::Dirs | QDir::NoSymLinks,QDir::SortFlags sortFlag=QDir::Name);

    QString readFileToQString(QString path,QString charset="utf-8",BizError* er=NULL);

    void writeQStringToFile(QString content,QString path,QString charset="utf-8",BizError* er=NULL);

    bool rename(const QString &source, const QString &newName);

    bool mv(const QString &source, const QString &destination);


    bool copy(const QString &source, const QString &destination, bool override);

    bool remove(const QString &dirName);

}


#endif // UTILS_H
