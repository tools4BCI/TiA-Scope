#include "data_receive_blocker.h"

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
void DataReceiveBlocker::receiveUDPData ()
{
    while (udp_data_socket_->hasPendingDatagrams())
    {
        QByteArray buffer (udp_data_socket_->pendingDatagramSize(), 0);
        udp_data_socket_->readDatagram (buffer.data(), buffer.size());
        data_stream_data_.append (buffer);
    }
    data_stream_data_wait_.wakeAll ();
}

//-----------------------------------------------------------------------------
void DataReceiveBlocker::receiveTCPData ()
{
    data_stream_data_.append(tcp_data_socket_->readAll());
    data_stream_data_wait_.wakeAll ();
}

}
