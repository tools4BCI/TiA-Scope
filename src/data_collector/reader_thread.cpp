#include "reader_thread.h"
#include "datapacket/data_packet.h"
#include "signalserver-client/ssconfig.h"
#include "config/ss_meta_info.h"
#include "definitions/constants.h"

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
    Constants constants;

    client_->startReceiving (udp_);

    while (client_->receiving() && running_)
    {
        client_->getDataPacket (packet);

        for (SignalInfo::SignalMap::const_iterator signal_iter = config.signal_info.signals().begin ();
             signal_iter != config.signal_info.signals().end ();
             ++signal_iter)
        {
            try
            {
                data = packet.getSingleDataBlock (constants.getSignalFlag (signal_iter->first));
            }
            catch (...)
            {
                data.clear();
            }

            if (data.size() == 0)
                continue;

            int blocksize = signal_iter->second.blockSize ();
            QList<double> list_data;
            for (int i = 0; i < blocksize; i++)
                list_data.append (i);
            for (unsigned channel_index = 0; channel_index < signal_iter->second.channels ().size ();
                 channel_index++)
            {
                for (int sample_index = 0; sample_index < blocksize; sample_index++)
                    list_data[sample_index] = data[(channel_index * blocksize) + sample_index];
                data_buffer_->appendData (signal_iter->first.c_str(), channel_index, list_data);
            }

        }
    }
    if (client_->receiving ())
        client_->stopReceiving ();
}

} }
