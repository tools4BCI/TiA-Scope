#ifndef TIA_CLIENT_BASED_LIBTIA_H
#define TIA_CLIENT_BASED_LIBTIA_H

#include "../qt_tia_client.h"


#include "tia_datapacket_basedlibtia.h"

#include "tia/tia_client.h"

#include "tia/data_packet_interface.h"

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------

class TiAQtClientBasedLibTiA : public TiAQtClient
{
public:
    TiAQtClientBasedLibTiA(bool use_new_tia);
    virtual ~TiAQtClientBasedLibTiA();

    virtual void connectToServer (QString server_address, unsigned port, bool udp_data_connection);

    virtual void connectToServer (QString server_address, unsigned port);

    inline void setDataConnectionType(bool udp_data_connection)
    {
        udp_data_connection_ = udp_data_connection;
    }

    virtual bool trySetCustomSignalInfo(tia::SignalInfo &custom_sig_info, std::string &error_msg);

    virtual void createDataConnection ();

    virtual bool connected();

    virtual void disconnectFromServer ();    

    virtual void requestMetaInfo();

    virtual tia::SSConfig getMetaInfo() const;

    virtual void startReceiving ();
    virtual void stopReceiving ();
    virtual QSharedPointer<DataPacket> getDataPacket ();

private:
    Q_DISABLE_COPY (TiAQtClientBasedLibTiA);

    tia::TiAClient tia_client_;    
    bool udp_data_connection_;

};

}

#endif // TIA_CLIENT_BASED_LIBTIA_H
