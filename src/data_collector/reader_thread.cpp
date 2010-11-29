#include "reader_thread.h"
#include "base/user_types.h"

#include "tia/data_packet.h"
#include "tia/ssconfig.h"
#include "tia/ss_meta_info.h"

#include <QDebug>
#include <iostream>

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
ReaderThread::ReaderThread (QSharedPointer<DataBuffer> data_buffer, SSClient* client, bool udp,
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
    DataPacket packet;
    std::vector<double> data;
    SSConfig config = client_->config();

    client_->startReceiving (udp_);

    while (client_->receiving() && running_)
    {
        client_->getDataPacket (packet);

        for (SignalInfo::SignalMap::const_iterator signal_iter = config.signal_info.signals().begin ();
             signal_iter != config.signal_info.signals().end ();
             ++signal_iter)
        {
            data.clear();
            SignalTypeFlag signal_flag = TypeConverter::stdStringToSignalTypeFlag (signal_iter->first);
            try
            {
                if (packet.getFlags () & signal_flag)
                    data = packet.getSingleDataBlock (signal_flag);
            }
            catch (...)
            {
                qDebug () << "getSingleDataBlock Exception for signal" << signal_iter->first.c_str();
                data.clear();
            }

            if (data.size() == 0)
                continue;

            if ((signal_flag == SIG_BUTTON) ||
                (signal_flag == SIG_JOYSTICK))
            {
                QList<double> values;
                for (int index = 1; index < data.size(); index++)
                {
                    values.append (data[index]);
                }
                data_buffer_->setAperiodicValues (signal_flag, data[0], values);
            }
            else
            {
                int blocksize = signal_iter->second.blockSize ();
                QList<double> list_data;
                for (int i = 0; i < blocksize; i++)
                    list_data.append (i);
                for (unsigned channel_index = 0; channel_index < signal_iter->second.channels ().size ();
                     channel_index++)
                {
                    for (int sample_index = 0; sample_index < blocksize; sample_index++)
                        list_data[sample_index] = data[(channel_index * blocksize) + sample_index];
                    data_buffer_->appendData (signal_flag, channel_index, list_data);
                }
            }
        }
    }
    if (client_->receiving ())
        client_->stopReceiving ();
}

} }
