#ifndef TIA_CLIENT_VERSION10_H
#define TIA_CLIENT_VERSION10_H

#include "../qt_tia_client.h"
#include "tia_control_message.h"
#include "data_receive_blocker.h"

#include <QDomDocument>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QMutex>
#include <QWaitCondition>

namespace TiAQtImplementation
{

class TiAQtClientVersion10 : public TiAQtClient
{
public:
    TiAQtClientVersion10();

    virtual ~TiAQtClientVersion10 () {}

    virtual void connectToServer (QString server_address, unsigned port, bool udp_data_connection);
    virtual void disconnectFromServer ();    

    virtual tia::SSConfig getMetaInfo() const;

    virtual void startReceiving ();
    virtual void stopReceiving ();
    virtual QSharedPointer<DataPacket> getDataPacket ();

private:
    Q_DISABLE_COPY (TiAQtClientVersion10);

    void buildMetaInfo ();
    void readSubjectInfo (QDomDocument& config_doc, QString key);
    void getDataConnection (bool udp);
    void getStateConnection ();
    TiAControlMessage callConfigCommand (QString const& command);
    QString readLine (QString awaited_start = "");

    static QString const GET_CONFIG_COMMAND_;
    static QString const GET_UDP_DATACONNECTION_COMMAND_;
    static QString const GET_TCP_DATACONNECTION_COMMAND_;
    static QString const START_COMMAND_;
    static QString const STOP_COMMAND_;
    static QString const VERSION_LINE_;
    static QString const GET_STATE_CONNECTION_COMMAND_;

    QTcpSocket control_socket_;
    QUdpSocket udp_data_socket_;
    QTcpSocket tcp_data_socket_;
    QTcpSocket state_socket_;
    QTextStream control_stream_;    

    tia::SSConfig tia_meta_info_;

    DataReceiveBlocker* receiver_;
    QByteArray data_stream_data_;
    QMutex data_stream_data_mutex_;
    QWaitCondition data_stream_data_wait_;
};

}

#endif // TIA_CLIENT_VERSION10_H
