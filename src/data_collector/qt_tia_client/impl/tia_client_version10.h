#ifndef TIA_CLIENT_VERSION10_H
#define TIA_CLIENT_VERSION10_H

#include "../qt_tia_client.h"
#include "tia_control_message.h"
#include "data_receive_blocker.h"

#include <QDomDocument>
#include <QTcpSocket>
#include <QMutex>
#include <QWaitCondition>

namespace TiAQtImplementation
{

class TiAQtClientVersion10 : public TiAQtClient
{
public:
    TiAQtClientVersion10();

    virtual ~TiAQtClientVersion10 () {}

    virtual void connectToServer (QString server_address, unsigned port);
    virtual void disconnectFromServer ();
    virtual TiAMetaInfo getMetaInfo () const;
    virtual void startReceiving ();
    virtual void stopReceiving ();
    virtual QSharedPointer<DataPacket> getDataPacket ();

private:
    Q_DISABLE_COPY (TiAQtClientVersion10);

    void buildMetaInfo ();
    void readSubjectInfo (QDomDocument& config_doc, QString key);
    void getDataConnection ();
    TiAControlMessage callConfigCommand (QString const& command);
    QString readLine (QString awaited_start = "");

    static QString const GET_CONFIG_COMMAND_;
    static QString const GET_DATACONNECTION_COMMAND_;
    static QString const START_COMMAND_;
    static QString const STOP_COMMAND_;
    static QString const VERSION_LINE_;

    QTcpSocket control_socket_;
    QTcpSocket data_socket_;
    QTextStream control_stream_;
    TiAMetaInfo meta_info_;
    DataReceiveBlocker* receiver_;
    QByteArray data_stream_data_;
    QMutex data_stream_data_mutex_;
    QWaitCondition data_stream_data_wait_;
};

}

#endif // TIA_CLIENT_VERSION10_H
