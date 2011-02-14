#ifndef DATA_RECEIVE_BLOCKER_H
#define DATA_RECEIVE_BLOCKER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QWaitCondition>


namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
class DataReceiveBlocker : public QObject
{
    Q_OBJECT
public:
    DataReceiveBlocker (QByteArray& data_stream_data , QUdpSocket* udp_data_socket,
              QWaitCondition& data_stream_data_wait) :
            data_stream_data_ (data_stream_data), udp_data_socket_ (udp_data_socket),
            data_stream_data_wait_ (data_stream_data_wait)
    {
        this->connect (udp_data_socket_, SIGNAL(readyRead()), SLOT(receiveUDPData()));
    }

    DataReceiveBlocker (QByteArray& data_stream_data , QTcpSocket* tcp_data_socket,
              QWaitCondition& data_stream_data_wait) :
            data_stream_data_ (data_stream_data), tcp_data_socket_ (tcp_data_socket),
            data_stream_data_wait_ (data_stream_data_wait)
    {
        this->connect (tcp_data_socket_, SIGNAL(readyRead()), SLOT(receiveTCPData()));
    }

private Q_SLOTS:
    void receiveUDPData ();
    void receiveTCPData ();

private:
    QByteArray& data_stream_data_;
    QUdpSocket* udp_data_socket_;
    QTcpSocket* tcp_data_socket_;
    QWaitCondition& data_stream_data_wait_;
    unsigned last_;
};

}

#endif // DATA_RECEIVE_BLOCKER_H
