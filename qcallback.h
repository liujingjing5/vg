#ifndef QCALLBACK_H
#define QCALLBACK_H

#include <boost/function.hpp>

class QCallback
{
public:
    QCallback(boost::function<void()> callbackFn);

private:
    boost::function<void()> callback_;
    void emit_callback();

public slots:
    void slot_callback();
signals:
    void signal_callback();
};

#endif // QCALLBACK_H
