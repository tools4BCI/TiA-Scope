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

namespace TiAQtImplementation
{

QString const TiAQtClientVersion02::GET_CONFIG_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>getConfig</type><sender></sender></header><getConfig/></message>";
QString const TiAQtClientVersion02::GET_DATACONNECTION_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>getDataConnection</type><sender/></header><getDataConnection><connectionType>tcp</connectionType></getDataConnection></message>";
QString const TiAQtClientVersion02::START_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>startTransmission</type><sender/></header><startTransmission /></message>";
QString const TiAQtClientVersion02::STOP_COMMAND_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><message version=\"0.1\"><header><type>stopTransmission</type><sender/></header><stopTransmission /></message>";

//-----------------------------------------------------------------------------
TiAQtClientVersion02::TiAQtClientVersion02 () : TiAQtClient ()
{
    // nothing to do here
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::connectToServer (QString server_address, unsigned port)
{
    control_socket_.connectToHost (server_address, port);
    if (!control_socket_.waitForConnected ())
        throw TiAException (control_socket_.errorString ());

    control_stream_.setDevice (&control_socket_);
    buildMetaInfo ();
    getDataConnection ();
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::disconnectFromServer ()
{
    //data_stream_.setDevice (0);
    data_socket_.disconnectFromHost ();
    control_stream_.setDevice (0);
    control_socket_.disconnectFromHost ();
    data_socket_.waitForDisconnected ();
    control_socket_.waitForDisconnected ();
}

//-----------------------------------------------------------------------------
TiAMetaInfo TiAQtClientVersion02::getMetaInfo () const
{
    return meta_info_;
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::startReceiving ()
{
    callConfigCommand (START_COMMAND_);
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::stopReceiving ()
{
    callConfigCommand (STOP_COMMAND_);
}

//-----------------------------------------------------------------------------
QSharedPointer<DataPacket> TiAQtClientVersion02::getDataPacket ()
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
void Receiver::receiveData ()
{
    data_stream_data_.append(data_socket_.readAll());
    data_stream_data_wait_.wakeAll ();
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
}

//-----------------------------------------------------------------------------
void TiAQtClientVersion02::getDataConnection ()
{
    QString data_connection_str = callConfigCommand (GET_DATACONNECTION_COMMAND_);
    QDomDocument data_con_doc;
    data_con_doc.setContent (data_connection_str);
    QDomNodeList ports = data_con_doc.elementsByTagName ("port");
    if (ports.size() != 1)
        throw TiAException ("Received no port number to establish a data connection to the server!");
    bool ok = false;
    quint16 port =  ports.item (0).toElement().text().toUShort(&ok);
    if (!ok)
        throw TiAException (QString ("Couldn't convert port string (received from server to establish a data connection) into a number. Port string is: ").append(ports.item(0).toElement().text()));

    data_socket_.connectToHost (control_socket_.peerAddress(), port);
    qDebug () << "readBufferSize = " << data_socket_.readBufferSize();
    data_socket_.waitForConnected ();
    receiver_ = new Receiver (data_stream_data_, data_socket_, data_stream_data_wait_);
}

//-----------------------------------------------------------------------------
QString TiAQtClientVersion02::callConfigCommand (QString const& command)
{
    control_stream_ << command;
    control_stream_.flush();

    QString message;
    while (!message.contains ("</message>"))
    {
        control_socket_.waitForReadyRead ();
        QString new_text = control_stream_.readAll ();
        message.append (new_text);
    }
    qDebug () << "received: " << message;
    return message;
}



}
