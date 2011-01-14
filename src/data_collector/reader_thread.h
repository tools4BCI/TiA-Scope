#ifndef READER_THREAD_H
#define READER_THREAD_H

#include "base/data_buffer.h"

#include "tia/tia_client.h"

#include <QThread>
#include <QSharedPointer>

namespace TiAScope
{

class ReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit ReaderThread (QSharedPointer<DataBuffer> data_buffer, tobiss::TiAClient* client, bool udp,
                           QObject *parent = 0);

    void stop ();
private:
    virtual void run ();

    QSharedPointer<DataBuffer> data_buffer_;
    tobiss::TiAClient* client_;
    bool running_;
    bool udp_;
};

} // TiAScope

#endif // READER_THREAD_H
