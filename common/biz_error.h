#ifndef BIZ_ERROR_H
#define BIZ_ERROR_H

#include<QString>

class BizError
{
public:
    BizError();
    BizError(int code,const QString& message);
    BizError(const QString& message);

    operator bool() const;

    void error(const QString& message);
    QString message();
    void reset();
public:
    const static int NORMAL=0;
    const static int UNKOWN=1;
private:
    int code_;
    QString message_;
};

#endif // BIZ_ERROR_H
