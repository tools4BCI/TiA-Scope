#ifndef DATA_RECEIVE_BLOCKER_H
#define DATA_RECEIVE_BLOCKER_H

#include <QObject>
#include <QTcpSocket>
#include <QWaitCondition>


namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
class DataReceiveBlocker : public QObject
{
    Q_OBJECT
public:
    DataReceiveBlocker (QByteArray& data_stream_data , QTcpSocket& data_socket,
              QWaitCondition& data_stream_data_wait) :
            data_stream_data_ (data_stream_data), data_socket_ (data_socket),
            data_stream_data_wait_ (data_stream_data_wait)
    {
        this->connect (&data_socket_, SIGNAL(readyRead()), SLOT(receiveData()));
    }

private Q_SLOTS:
    void receiveData ();

private:
    QByteArray& data_stream_data_;
    QTcpSocket& data_socket_;
    QWaitCondition& data_stream_data_wait_;
    unsigned last_;
};

}

#endif // DATA_RECEIVE_BLOCKER_H
