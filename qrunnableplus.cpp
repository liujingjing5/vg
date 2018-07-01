#include "qrunnableplus.h"

QRunnablePlus::QRunnablePlus(boost::function<void()> callback):QRunnable(),callback_(callback)
{
}

void QRunnablePlus::run()
{
    callback_();
}
