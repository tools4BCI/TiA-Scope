#include "reader_thread.h"
#include "base/user_types.h"

#include "qt_tia_client/tia_exception.h"

#include <QDebug>
#include <QMutexLocker>
#include <iostream>
#include <QMessageBox>

namespace TiAScope {

//-----------------------------------------------------------------------------
ReaderThread::ReaderThread (QSharedPointer<DataBuffer> data_buffer, QSharedPointer<TiAQtImplementation::TiAQtClient> client, bool udp,
                            QObject *parent) :
    QThread(parent),
    data_buffer_ (data_buffer),
    client_ (client),
    udp_ (udp)
{
    setObjectName (QString("ReaderThread 0x").append(QString::number(reinterpret_cast<int64_t const>(this), 16)));
}

//-----------------------------------------------------------------------------
void ReaderThread::stop ()
{
    running_mutex_.lock();
    running_ = false;
    running_mutex_.unlock();;
}

//-----------------------------------------------------------------------------
void ReaderThread::run ()
{
    running_mutex_.lock();
    running_ = true;
    running_mutex_.unlock();
    try
    {
        client_->startReceiving();

        running_mutex_.lock();
        while (running_)
        {
            running_mutex_.unlock();
            QSharedPointer<TiAQtImplementation::DataPacket> datapacket = client_->getDataPacket();
            if (datapacket.isNull())
            {
                running_ = false;
            }
            else
            {
                Q_FOREACH (TiAQtImplementation::SignalTypeFlag signal_flag, datapacket->getSignals())
                {
                    if (TiAQtImplementation::isAperiodic (signal_flag))
                    {
                        // todo: handle aperiodic signals!
                    }
                    else
                    {
                        for (TiAQtImplementation::ChannelIndex channel_index = 0; channel_index < datapacket->getNumChannels(signal_flag); ++channel_index)
                            data_buffer_->appendData (signal_flag, channel_index, datapacket->getData (signal_flag, channel_index).toList());
                    }
                }
            }
            running_mutex_.lock();
        }
        running_mutex_.unlock();
        client_->stopReceiving ();
    }
    catch (TiAQtImplementation::TiAException &exc)
    {
        qDebug () << exc.what();
        QMessageBox::critical (0, "Error", exc.what());
    }

    qDebug () << __FUNCTION__ << "finished";
}

} // TiAScope
