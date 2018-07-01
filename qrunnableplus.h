#ifndef QRUNNABLEPLUS_H
#define QRUNNABLEPLUS_H

#include <QRunnable>
#include <functional>
#include <string>
#include <boost/function.hpp>

class QRunnablePlus:public QRunnable
{
public:
    QRunnablePlus(boost::function<void()> callback);
    virtual void run();
private:
    boost::function<void()> callback_;
};

#endif // QRUNNABLEPLUS_H
