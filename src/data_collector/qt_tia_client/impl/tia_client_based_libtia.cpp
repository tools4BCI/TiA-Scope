#include "tia_client_based_libtia.h"

//bad hack to undefine signals that is used by Qt
//but at the same time defines a method of SignalInfo
//used in libTiA
#undef signals

#include "tia/ssconfig.h"

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
    tia_client_.connect(server_address.toStdString(),port);

    tia_client_.requestConfig();

    //TODO: add possibility to connect to the servers state connection

    tia_client_.createDataConnection(udp_data_connection);

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

    //TODO: check null pointer!

    tia_client_.getDataPacket(*tia_packet);

    QSharedPointer<DataPacket> packet (new DataPacketBasedLibTiA(*tia_packet));

    return packet;
}

//-----------------------------------------------------------------------------

TiAMetaInfo TiAQtClientBasedLibTiA::getMetaInfo() const
{
//    tia_client_.config()
    const tia::SSConfig& ss_config = tia_client_.config();

//    const tia::SignalInfo::SignalMap &signal_map = ss_config.signal_info.signals();

//    const tia::SignalInfo::SignalMap::iterator signal = signal_map.begin();

//    for(; signal != signal_map.end(); ++signal)
//    {
//        signal->
//    }

//    ss_config.subject_info_;
}

tia::SSConfig TiAQtClientBasedLibTiA::getTiaMetaInfo() const
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
