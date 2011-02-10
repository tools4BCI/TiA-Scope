#ifndef READER_THREAD_H
#define READER_THREAD_H

#include "base/data_buffer.h"

#include "qt_tia_client/qt_tia_client.h"

#include <QThread>
#include <QSharedPointer>
#include <QMutex>

namespace TiAScope
{

class ReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit ReaderThread (QSharedPointer<DataBuffer> data_buffer, QSharedPointer<TiAQtImplementation::TiAQtClient> client, bool udp,
                           QObject *parent = 0);

    void stop ();
private:
    virtual void run ();

    QMutex running_mutex_;
    QSharedPointer<DataBuffer> data_buffer_;
    QSharedPointer<TiAQtImplementation::TiAQtClient> client_;
    bool running_;
    bool udp_;
};

} // TiAScope

#endif // READER_THREAD_H
