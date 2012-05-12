#ifndef SCOPE_TIA_CLIENT_H
#define SCOPE_TIA_CLIENT_H

#include "tia_metainfo.h"
#include "tia/ssconfig.h"

#include "datapacket.h"


#include <QString>
#include <QSharedPointer>

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

    virtual void connectToServer (QString server_address, unsigned port, bool udp_data_connection) = 0;
    virtual void disconnectFromServer () = 0;    

    virtual tia::SSConfig getMetaInfo () const = 0;

    virtual void startReceiving () = 0;
    virtual void stopReceiving () = 0;
    virtual QSharedPointer<DataPacket> getDataPacket () = 0;

protected:
    TiAQtClient () {}

private:
    Q_DISABLE_COPY (TiAQtClient);
};

}

#endif // SCOPE_TIA_CLIENT_H
