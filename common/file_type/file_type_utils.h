#ifndef FILE_TYPE_UTILS_H
#define FILE_TYPE_UTILS_H

#include<QString>

namespace FileTypeUtils {

class FileTypeKey{
public:
    FileTypeKey(const QString& key);
    ~FileTypeKey();
    bool operator<(const FileTypeKey& key) const;
private:
    QString key_;
};


class FileType{
public:
    QString type;
    QString characterSet;
};

FileType findFileType(const QString& hexStr);
FileType findFileTypeByFilePath(const QString& filePath);

}

#endif // FILE_TYPE_UTILS_H
