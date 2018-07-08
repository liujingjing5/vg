#include "biz_error.h"

BizError::BizError():code_(NORMAL)
{

}

BizError::BizError(int code, const QString &message):code_(code),message_(message)
{

}

BizError::BizError(const QString &message):code_(UNKOWN),message_(message)
{

}

BizError::operator bool() const
{
    return code_ != 0;
}

void BizError::error(const QString &message)
{
    code_ = UNKOWN;
    message_ = message;
}

QString BizError::message()
{
    return message_;
}

void BizError::reset()
{
    code_=NORMAL;
    message_.clear();
}
