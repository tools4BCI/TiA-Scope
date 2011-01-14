#ifndef SCOPE_TIA_CLIENT_H
#define SCOPE_TIA_CLIENT_H

#include <QString>
#include <QTcpSocket>

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
///
/// TiAQtClient
///
/// base class for Qt based TiA client implementations
class TiAQtClient
{
public:
    virtual ~TiAQtClient () {}

    virtual void connect (QString server_address, unsigned port) = 0;
    virtual void disconnect () = 0;
    // virtual void getDataPacket () = 0;
    // virtual void getConfig () = 0;

protected:
    TiAQtClient () {}

private:
    Q_DISABLE_COPY (TiAQtClient);

    QTcpSocket control_connection_socket_;
};

}

#endif // SCOPE_TIA_CLIENT_H
