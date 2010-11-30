#ifndef READER_THREAD_H
#define READER_THREAD_H

#include "base/data_buffer.h"

#include "tia/tia_client.h"

#include <QThread>
#include <QSharedPointer>

namespace tobiss { namespace scope {

class ReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit ReaderThread (QSharedPointer<DataBuffer> data_buffer, TiAClient* client, bool udp,
                           QObject *parent = 0);

    void stop ();
private:
    virtual void run ();

    QSharedPointer<DataBuffer> data_buffer_;
    TiAClient* client_;
    bool running_;
    bool udp_;
};

} } // namespace

#endif // READER_THREAD_H
