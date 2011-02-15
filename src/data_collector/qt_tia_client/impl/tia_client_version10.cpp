#include "tia_client_version10.h"
#include "../tia_exception.h"
#include "tia_datapacket_version2.h"
#include "line_receive_blocker.h"

#include <QMutexLocker>
#include <QStringList>
#include <QHostAddress>
#include <QTime>

namespace TiAQtImplementation
{

QString const TiAQtClientVersion10::GET_CONFIG_COMMAND_ = "TiA 1.0\nGetMetaInfo\n\n";
QString const TiAQtClientVersion10::GET_UDP_DATACONNECTION_COMMAND_ = "TiA 1.0\nGetDataConnection: UDP\n\n";
QString const TiAQtClientVersion10::GET_TCP_DATACONNECTION_COMMAND_ = "TiA 1.0\nGetDataConnection: TCP\n\n";
QString const TiAQtClientVersion10::START_COMMAND_ = "TiA 1.0\nStartDataTransmission\n\n";
QString const TiAQtClientVersion10::STOP_COMMAND_ = "TiA 1.0\nStopDataTransmission\n\n";


//-----------------------------------------------------------------------------
TiAQtClientVersion10::TiAQtClientVersion10 () : receiver_ (0)
{
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion10::connectToServer (QString server_address, unsigned port, bool udp_data_connection)
{
    control_socket_.connectToHost (server_address, port);
    if (!control_socket_.waitForConnected ())
        throw TiAException (control_socket_.errorString ());

    control_stream_.setDevice (&control_socket_);
    buildMetaInfo ();
    getDataConnection (udp_data_connection);
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion10::disconnectFromServer ()
{
    udp_data_socket_.disconnectFromHost ();
    tcp_data_socket_.disconnectFromHost ();
    control_stream_.setDevice (0);
    control_socket_.disconnectFromHost ();
    udp_data_socket_.waitForDisconnected ();
    tcp_data_socket_.waitForDisconnected ();
    control_socket_.waitForDisconnected ();
}

//-----------------------------------------------------------------------------
TiAMetaInfo TiAQtClientVersion10::getMetaInfo () const
{
    return meta_info_;
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion10::startReceiving ()
{
    callConfigCommand (START_COMMAND_);
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion10::stopReceiving ()
{
    callConfigCommand (STOP_COMMAND_);
}

//-----------------------------------------------------------------------------
QSharedPointer<DataPacket> TiAQtClientVersion10::getDataPacket ()
{
    QMutexLocker locker (&data_stream_data_mutex_);
    unsigned needed = DataPacketVersion2::canCreate (data_stream_data_);
    while (needed == 0)
    {
        data_stream_data_wait_.wait (&data_stream_data_mutex_);
        needed = DataPacketVersion2::canCreate (data_stream_data_);
    }

    QByteArray packet = data_stream_data_.left(needed);
    data_stream_data_.remove(0, needed);
    QSharedPointer<DataPacket> datapacket (new DataPacketVersion2 (packet));
    return datapacket;
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion10::buildMetaInfo ()
{
    TiAControlMessage config = callConfigCommand (GET_CONFIG_COMMAND_);

    QDomDocument config_doc;
    config_doc.setContent (config.content);
    QDomNodeList signal_nodes = config_doc.elementsByTagName ("signal");

    for (int node_index = 0; node_index < signal_nodes.count(); node_index++)
    {
        QDomElement signal_element = signal_nodes.item (node_index).toElement ();
        QString signal_type = signal_element.attribute ("type");

        SignalTypeFlag singal_type_flag = toSignalTypeFlag (signal_type);

        meta_info_.addSignal (singal_type_flag, signal_element.attribute("numChannels").toUInt());
        meta_info_.setSamplingRate (singal_type_flag, signal_element.attribute("samplingRate").toDouble());

        QDomNodeList channel_nodes = signal_element.elementsByTagName ("channel");
        for (int channel_index = 0; channel_index < channel_nodes.size(); channel_index++)
        {
            QDomElement channel_element = channel_nodes.item (channel_index).toElement ();
            meta_info_.setChannelLabel (singal_type_flag, channel_element.attribute ("nr").toUInt() - 1, channel_element.attribute ("label"));
        }
    }

    // build subject info
//    readSubjectInfo (config_doc, "firstName");
//    readSubjectInfo (config_doc, "surname");
//    readSubjectInfo (config_doc, "birthday");
//    readSubjectInfo (config_doc, "handedness");
}
//-----------------------------------------------------------------------------
void TiAQtClientVersion10::readSubjectInfo (QDomDocument& /*config_doc*/, QString /*key*/)
{
//    QDomNodeList key_nodes = config_doc.elementsByTagName (key);
//    if (key_nodes.size())
//        meta_info_.addSubjectInfoEntry (key, key_nodes.item (0).toElement().text());
}


//-----------------------------------------------------------------------------
void TiAQtClientVersion10::getDataConnection (bool udp)
{
    if (udp)
    {
        TiAControlMessage data_connection_message = callConfigCommand (GET_UDP_DATACONNECTION_COMMAND_);
        udp_data_socket_.bind (data_connection_message.parameter.toUInt(), QUdpSocket::ShareAddress);
        receiver_ = new DataReceiveBlocker (data_stream_data_, &udp_data_socket_, data_stream_data_wait_);
    }
    else
    {
        TiAControlMessage data_connection_message = callConfigCommand (GET_TCP_DATACONNECTION_COMMAND_);
        tcp_data_socket_.connectToHost (control_socket_.peerAddress(), data_connection_message.parameter.toUInt());
        tcp_data_socket_.waitForConnected ();
        receiver_ = new DataReceiveBlocker (data_stream_data_, &tcp_data_socket_, data_stream_data_wait_);
    }

}

//-----------------------------------------------------------------------------
TiAControlMessage TiAQtClientVersion10::callConfigCommand (QString const& command)
{
    control_stream_ << command;
    control_stream_.flush();

    TiAControlMessage message;
    message.version = readLine (QString ("TiA 1.0"));

    if (!message.version.contains ("TiA 1.0"))
        throw TiAException (QString("Wrong reply message version. Received: ").append (message.version));

    message.command = readLine ();
    if (message.command.contains(":"))
    {
        QStringList list = message.command.split (":");
        message.command = list.first();
        message.parameter = list.last();
    }

    QString optional_content = readLine ();
    int content_length = 0;
    if (optional_content.size())
    {
        QStringList list = optional_content.split(":");
        if (list.first() != "Content-Length")
            throw TiAException ("Unknown field! Expected Content-Length field.");
        content_length = list.last().toUInt ();

        QString empty_line = readLine ();
        if (empty_line.size())
            throw TiAException ("Expected empty line but was ");
    }

    while (message.content.size() < content_length)
    {
        message.content.append (control_stream_.read (content_length - message.content.size()));
        if (message.content.size() < content_length)
            control_socket_.waitForReadyRead (10);
    }

    return message;
}

//-----------------------------------------------------------------------------
QString TiAQtClientVersion10::readLine (QString awaited_start)
{
    QString line;
    char data;
    QTime time;
    time.restart ();
    while (!control_socket_.read (&data, 1))
    {
        if (time.elapsed() > 1000)
            throw TiAException (QString ("Server does not respond to TiA 1.0."));
        control_socket_.waitForReadyRead (10);
    }

    if (awaited_start.size())
        if (awaited_start[0] != data)
            throw TiAException (QString("Unexpected character! Expected:").append(awaited_start).append(" but received ").append(data));

    while (data != 0x0a)
    {
        line.append (data);
        while (!control_socket_.read (&data, 1))
            control_socket_.waitForReadyRead (10);
    }
    return line;
}


}
