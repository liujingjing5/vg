#include "utils.h"
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QProcess>

namespace utils{


void _listFileInfoList(QFileInfoList& fileInfoList, QDir dirpath){
    const QFileInfoList templist =dirpath.entryInfoList();
    foreach (QFileInfo tempfileInfo, templist) {
        if(tempfileInfo.isFile()){
            fileInfoList.append(tempfileInfo);
        }else if(tempfileInfo.isDir() && tempfileInfo.fileName()!="." && tempfileInfo.fileName()!=".."){
            QDir tempDir(tempfileInfo.filePath());
            tempDir.setFilter(dirpath.filter());
            tempDir.setSorting(dirpath.sorting());
            _listFileInfoList(fileInfoList,tempDir);
        }
    }

}

QFileInfoList listFileInfoList(QString dirpath,QDir::Filters findFlag, QDir::SortFlags sortFlag)
{
    QFileInfoList fileInfoList;

    QDir templateDir(dirpath);
    templateDir.setFilter(findFlag);
    templateDir.setSorting(sortFlag);
    _listFileInfoList(fileInfoList,templateDir);
    return fileInfoList;
}

QString readFileToQString(QString path, QString charset)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return "";
    }else{
        QByteArray data = file.readAll();
        if(charset.toLower()=="gbk"){
            QTextCodec *codec = QTextCodec::codecForName("gbk");
            return codec->toUnicode(data);
        }else{
            return QString::fromUtf8(data);
        }
    }


}

void writeQStringToFile(QString content, QString path, QString charset)
{
    QFileInfo fileInfo(path);
    QFile file(path);
    fileInfo.dir().mkdir(".");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){

    }else{
        if(charset.toLower()=="gbk"){
            QTextCodec *codec = QTextCodec::codecForName("gbk");
            QByteArray data = codec->fromUnicode(content);
            file.write(data);
        }else{
            QByteArray data = content.toUtf8();
            file.write(data);
        }
    }
}


bool copy(const QString &source, const QString &destination, bool override)
{
    if (source == destination){
        return false;
    }
    QFileInfo sourceInfo(source);
    if(sourceInfo.isFile()){
        if (QFile::exists(destination)){
            if(override){
                QFile::remove(destination);
            }
        }
        return QFile::copy(source, destination);
    }else if(sourceInfo.isDir()){
        QDir sourceDir(source);
        QDir destinatDir(destination);
        if (!sourceDir.exists())
        {
            return false;
        }
        destinatDir.mkpath(".");
        bool error = false;
        QStringList fileNames = sourceDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        for (QStringList::size_type i=0; i != fileNames.size(); ++i)
        {
            QString fileName = fileNames.at(i);
            QString srcFilePath = sourceDir.filePath(fileName);
            QString dstFilePath = destinatDir.filePath(fileName);
            QFileInfo fileInfo(srcFilePath);
            if (fileInfo.isFile())
            {
                if (override)
                {
                    QFile::setPermissions(dstFilePath, QFile::WriteOwner);
                }
                QFile::copy(srcFilePath, dstFilePath);
            }
            else if (fileInfo.isDir())
            {
                QDir(dstFilePath).mkpath(".");
                if (!utils::copy(srcFilePath, dstFilePath, override))
                {
                    error = true;
                }
            }
        }
        return !error;
    }
    return false;

}

bool remove(const QString &source)
{
    QFileInfo sourceInfo(source);
    if(sourceInfo.isFile()){
        return QFile::remove(source);
    }else if(sourceInfo.isDir()){
        QDir directory(source);
        if (!directory.exists())
        {
            return true;
        }

        QString srcPath = QDir::toNativeSeparators(source);
        if (!srcPath.endsWith(QDir::separator()))
            srcPath += QDir::separator();

        QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        bool error = false;
        for (QStringList::size_type i=0; i != fileNames.size(); ++i)
        {
            QString filePath = srcPath + fileNames.at(i);
            QFileInfo fileInfo(filePath);
            if (fileInfo.isFile() || fileInfo.isSymLink())
            {
                QFile::setPermissions(filePath, QFile::WriteOwner);
                if (!QFile::remove(filePath))
                {
//                    qDebug() << "remove file" << filePath << " faild!";
                    error = true;
                }
            }
            else if (fileInfo.isDir())
            {
                if (!utils::remove(filePath))
                {
                    error = true;
                }
            }
        }

        if (!directory.rmdir("."))
        {
//            qDebug() << "remove dir" << directory.path() << " faild!";
            error = true;
        }

        return !error;
    }
}

bool rename(const QString &source, const QString &newName)
{
    QFileInfo sourceInfo(source);
    QDir parentDir = sourceInfo.dir();
    return parentDir.rename(sourceInfo.fileName(),newName);
}

bool mv(const QString &source, const QString &destination)
{
    //TODO 这个实现的 不好
    if(utils::copy(source,destination,true)){
        return utils::remove(source);
    }
    return false;
}

QString file(const QString &filePath)
{
    QProcess process;
    process.start("file", QStringList()<< filePath );
    process.waitForFinished();
    QString outStr = QString(process.readAllStandardOutput());
    return outStr;
}



}

