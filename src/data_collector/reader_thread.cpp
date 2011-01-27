#include "reader_thread.h"
#include "base/user_types.h"

#include "qt_tia_client/tia_metainfo.h"

#include <QDebug>
#include <iostream>

namespace TiAScope {

//-----------------------------------------------------------------------------
ReaderThread::ReaderThread (QSharedPointer<DataBuffer> data_buffer, QSharedPointer<TiAQtImplementation::TiAQtClient> client, bool udp,
                            QObject *parent) :
    QThread(parent),
    data_buffer_ (data_buffer),
    client_ (client),
    udp_ (udp)
{
    setObjectName (QString("ReaderThread 0x").append(QString::number(reinterpret_cast<int const>(this), 16)));
}

//-----------------------------------------------------------------------------
void ReaderThread::stop ()
{
    running_ = false;
}

//-----------------------------------------------------------------------------
void ReaderThread::run ()
{
    running_ = true;
    TiAQtImplementation::TiAMetaInfo config = client_->getMetaInfo();

    client_->startReceiving();

    while (running_)
    {
        QSharedPointer<TiAQtImplementation::DataPacket> datapacket = client_->getDataPacket();
        Q_FOREACH (TiAQtImplementation::SignalTypeFlag signal_flag, datapacket->getSignals())
        {
            if (signal_flag & (TiAQtImplementation::SIGNAL_TYPE_Buttons | TiAQtImplementation::SIGNAL_TYPE_Joystick))
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
    client_->stopReceiving ();
}

} // TiAScope
