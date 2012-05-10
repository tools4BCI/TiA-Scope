#include "tia_client_version02.h"
#include "tia_datapacket_version2.h"
#include "../tia_exception.h"

#include <QTextCodec>
#include <QDebug>
#include <QDomDocument>
#include <QHostAddress>
#include <QThread>
#include <QTime>
#include <QMutexLocker>

#include "tia-private/config/control_message_decoder.h"
#include <sstream>

namespace TiAQtImplementation
{

QString const TiAQtClientVersion02::GET_CONFIG_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>getConfig</type><sender></sender></header><getConfig/></message>";
QString const TiAQtClientVersion02::GET_UDP_DATACONNECTION_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>getDataConnection</type><sender/></header><getDataConnection><connectionType>udp</connectionType></getDataConnection></message>";
QString const TiAQtClientVersion02::GET_TCP_DATACONNECTION_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>getDataConnection</type><sender/></header><getDataConnection><connectionType>tcp</connectionType></getDataConnection></message>";
QString const TiAQtClientVersion02::START_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>startTransmission</type><sender/></header><startTransmission /></message>";
QString const TiAQtClientVersion02::STOP_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>stopTransmission</type><sender/></header><stopTransmission /></message>";

//-----------------------------------------------------------------------------
TiAQtClientVersion02::TiAQtClientVersion02 () : TiAQtClient (),
    receiver_ (0),
    receiving_ (false)
{
    // nothing to do here
}

//-----------------------------------------------------------------------------
TiAQtClientVersion02::~TiAQtClientVersion02 ()
{
    delete receiver_;
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::connectToServer (QString server_address, unsigned port, bool udp_data_connection)
{
    control_socket_.connectToHost (server_address, port);
    if (!control_socket_.waitForConnected ())
        throw TiAException (control_socket_.errorString ());

    control_stream_.setDevice (&control_socket_);
    buildMetaInfo ();
    getDataConnection (udp_data_connection);
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::disconnectFromServer ()
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
TiAMetaInfo TiAQtClientVersion02::getMetaInfo () const
{
    return meta_info_;
}

//-----------------------------------------------------------------------------

tia::SSConfig TiAQtClientVersion02::getTiaMetaInfo() const
{
    return tia_meta_info_;
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::startReceiving ()
{
    callConfigCommand (START_COMMAND_);
    receiving_ = true;
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::stopReceiving ()
{
    callConfigCommand (STOP_COMMAND_);
    receiving_ = false;
}

//-----------------------------------------------------------------------------
QSharedPointer<DataPacket> TiAQtClientVersion02::getDataPacket ()
{
    QMutexLocker locker (&data_stream_data_mutex_);
    unsigned needed = DataPacketVersion2::canCreate (data_stream_data_);
    while ((needed == 0) && receiving_)
    {
        data_stream_data_wait_.wait (&data_stream_data_mutex_);
        if (!receiving_)
            return QSharedPointer<DataPacket> (0);
        needed = DataPacketVersion2::canCreate (data_stream_data_);
    }
    if (!receiving_)
        return QSharedPointer<DataPacket> (0);

    QByteArray packet = data_stream_data_.left(needed);
    data_stream_data_.remove(0, needed);
    QSharedPointer<DataPacket> datapacket (new DataPacketVersion2 (packet));
    return datapacket;
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::buildMetaInfo ()
{
    QString config = callConfigCommand (GET_CONFIG_COMMAND_);

    QDomDocument config_doc;
    config_doc.setContent (config);
    QDomNodeList signal_nodes = config_doc.elementsByTagName ("sig");

    qDebug () << "Signals: ";
    for (int node_index = 0; node_index < signal_nodes.count(); node_index++)
    {
        QDomElement signal_element = signal_nodes.item (node_index).toElement ();
        QDomElement samplingrate_element = signal_element.firstChildElement ("samplingRate");
        QString signal_type = signal_element.attribute ("type");

        SignalTypeFlag singal_type_flag = toSignalTypeFlag (signal_type);

        QDomNodeList channel_nodes = signal_element.elementsByTagName ("ch");
        meta_info_.addSignal (singal_type_flag, channel_nodes.size ());
        meta_info_.setSamplingRate (singal_type_flag, samplingrate_element.text().toDouble());
        for (int channel_index = 0; channel_index < channel_nodes.size(); channel_index++)
        {
            QDomElement channel_element = channel_nodes.item (channel_index).toElement ();
            qDebug () << "    -" << channel_element.attribute ("id");
            meta_info_.setChannelLabel (singal_type_flag, channel_index, channel_element.attribute ("id"));
        }
    }

    // build subject info
    readSubjectInfo (config_doc, "firstName");
    readSubjectInfo (config_doc, "surname");
    readSubjectInfo (config_doc, "birthday");
    readSubjectInfo (config_doc, "handedness");


    std::stringstream ss;

    tia::ControlMsgDecoderXML msg_decoder;

    msg_decoder.setInputStream(&ss);

    ss << config.toStdString();

//    std::cout << ss.str() << std::endl;

    tia::ControlMsg *reply = msg_decoder.decodeMsg();

    if (reply == 0)
    {
      throw TiAException (QString ("TiAQtClientVersion02: Can't decode meta info."));
    }

    // Check reply type
    switch (reply->msgType())
    {
      case tia::ControlMsg::Config: break;

      case tia::ControlMsg::ErrorReply:
      {
        throw TiAException (QString ("TiAQtClientVersion02: Getting the config failed due to a server error."));
      }
      default:
      {
        throw TiAException (QString ("TiAQtClientVersion02: Got unexpected reply of type : ").append(reply->msgType()));
      }
    }

    tia::ConfigMsg *config_msg = static_cast<tia::ConfigMsg *>(reply);

    tia_meta_info_.subject_info = config_msg->subject_info;
    tia_meta_info_.signal_info = config_msg->signal_info;

    delete config_msg;

}
//-----------------------------------------------------------------------------
void TiAQtClientVersion02::readSubjectInfo (QDomDocument& config_doc, QString key)
{
    QDomNodeList key_nodes = config_doc.elementsByTagName (key);
    if (key_nodes.size())
        meta_info_.addSubjectInfoEntry (key, key_nodes.item (0).toElement().text());
}


//-----------------------------------------------------------------------------
void TiAQtClientVersion02::getDataConnection (bool udp)
{
    QString data_connection_str;
    if (udp)
        data_connection_str = callConfigCommand (GET_UDP_DATACONNECTION_COMMAND_);
    else
        data_connection_str = callConfigCommand (GET_TCP_DATACONNECTION_COMMAND_);

    QDomDocument data_con_doc;
    data_con_doc.setContent (data_connection_str);
    QDomNodeList ports = data_con_doc.elementsByTagName ("port");
    if (ports.size() != 1)
        throw TiAException ("Received no port number to establish a data connection to the server!");
    bool ok = false;
    quint16 port =  ports.item (0).toElement().text().toUShort(&ok);
    if (!ok)
        throw TiAException (QString ("Couldn't convert port string (received from server to establish a data connection) into a number. Port string is: ").append(ports.item(0).toElement().text()));

    if (udp)
    {
        udp_data_socket_.bind (port, QUdpSocket::ShareAddress);
        receiver_ = new DataReceiveBlocker (data_stream_data_, &udp_data_socket_, data_stream_data_wait_);
    }
    else
    {
        tcp_data_socket_.connectToHost (control_socket_.peerAddress(), port, QAbstractSocket::ReadOnly);
        tcp_data_socket_.waitForConnected ();
        receiver_ = new DataReceiveBlocker (data_stream_data_, &tcp_data_socket_, data_stream_data_wait_);
    }
}

//-----------------------------------------------------------------------------
QString TiAQtClientVersion02::callConfigCommand (QString const& command)
{
    control_stream_ << command;
    control_stream_.flush();
    qDebug () << "Send message: " << command;

    QTime time;
    QString message;
    time.restart();
    while ((!message.contains ("</message>")) && (time.elapsed() < 2000))
    {
        control_socket_.waitForReadyRead (100);
        QString new_text = control_stream_.readAll ();
        message.append (new_text);
    }
    if (time.elapsed() > 2000)
    {
        throw TiAException ("Server did not respond within 2s.");
    }
    qDebug () << "received: " << message;
    return message;
}



}
