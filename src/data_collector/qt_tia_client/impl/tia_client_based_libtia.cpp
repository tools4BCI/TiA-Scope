#include "tia_client_based_libtia.h"

//bad hack to undefine signals that is used by Qt
//but at the same time defines a method of SignalInfo
//used in libTiA
#undef signals

#include "tia/ssconfig.h"
#include "data_collector/qt_tia_client/tia_exception.h"

using namespace TiAQtImplementation;

//-----------------------------------------------------------------------------

TiAQtClientBasedLibTiA::TiAQtClientBasedLibTiA(bool use_new_tia)
    : tia_client_(use_new_tia)
{
}

//-----------------------------------------------------------------------------

TiAQtClientBasedLibTiA::~TiAQtClientBasedLibTiA()
{
}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::connectToServer(QString server_address, unsigned port, bool udp_data_connection)
{
    connectToServer(server_address,port);

    udp_data_connection_ = udp_data_connection;

    createDataConnection();

}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::connectToServer(QString server_address, unsigned port)
{
    tia_client_.connect(server_address.toStdString(),port);

    tia_client_.requestConfig();

    //TODO: add possibility to connect to the servers state connection
}

//-----------------------------------------------------------------------------

bool TiAQtClientBasedLibTiA::trySetCustomSignalInfo(tia::SignalInfo &custom_sig_info, std::string &error_msg)
{
    return tia_client_.trySetCustomSignalInfo(custom_sig_info, error_msg);
}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::createDataConnection()
{
    tia_client_.createDataConnection(udp_data_connection_);
}

//-----------------------------------------------------------------------------

bool TiAQtClientBasedLibTiA::connected()
{
    return tia_client_.connected();
}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::disconnectFromServer()
{
    tia_client_.disconnect();
}

//-----------------------------------------------------------------------------

QSharedPointer<DataPacket> TiAQtClientBasedLibTiA::getDataPacket()
{
    tia::DataPacket *tia_packet = tia_client_.getEmptyDataPacket();    

    if(tia_packet == NULL)
        throw new TiAException("TiaQtClient::getDataPacket(): Got null pointer from TiaClient!");

    tia_client_.getDataPacket(*tia_packet);

    QSharedPointer<DataPacket> packet (new DataPacketBasedLibTiA(*tia_packet));

    return packet;
}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::requestMetaInfo()
{
    tia_client_.requestConfig();
}

//-----------------------------------------------------------------------------
tia::SSConfig TiAQtClientBasedLibTiA::getMetaInfo() const
{
    return tia_client_.config();
}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::startReceiving()
{
    tia_client_.startReceiving();
}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::stopReceiving()
{
    tia_client_.stopReceiving();
}

//revert hack that signals is undefined
#define signals Q_SIGNALS
