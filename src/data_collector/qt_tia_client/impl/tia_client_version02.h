#ifndef TIA_CLIENT_VERSION02_H
#define TIA_CLIENT_VERSION02_H

#include "../qt_tia_client.h"

#include <QTcpSocket>
#include <QTextStream>
#include <QDataStream>

namespace TiAQtImplementation
{

class Receiver;

//-----------------------------------------------------------------------------
///
/// TiAQtClientVersion02
///
/// communicates with the TiA xml-messages version 0.2 and datapackets v2
class TiAQtClientVersion02 : public TiAQtClient
{
public:
    TiAQtClientVersion02 ();
    virtual void connectToServer (QString server_address, unsigned port);
    virtual void disconnectFromServer ();
    virtual TiAMetaInfo getMetaInfo () const;
    virtual void startReceiving ();
    virtual void stopReceiving ();
    virtual QSharedPointer<DataPacket> getDataPacket ();

private:
    void buildMetaInfo ();
    void getDataConnection ();
    QString callConfigCommand (QString const& command);

    Q_DISABLE_COPY (TiAQtClientVersion02);
    QTcpSocket control_socket_;
    QTcpSocket data_socket_;
    QTextStream control_stream_;
    TiAMetaInfo meta_info_;

    static QString const GET_CONFIG_COMMAND_;
    static QString const GET_DATACONNECTION_COMMAND_;
    static QString const START_COMMAND_;
    static QString const STOP_COMMAND_;

    QByteArray data_stream_data_;
    Receiver* receiver_;
};

class Receiver : public QObject
{
    Q_OBJECT
public:
    Receiver (QByteArray& data_stream_data , QTcpSocket& data_socket) :
            data_stream_data_ (data_stream_data), data_socket_ (data_socket)
    {
        this->connect (&data_socket_, SIGNAL(readyRead()), SLOT(receiveData()));
    }

private Q_SLOTS:
    void receiveData ();

private:
    QByteArray& data_stream_data_;
    QTcpSocket& data_socket_;
    unsigned last_;
};


}

#endif // TIA_CLIENT_VERSION02_H
