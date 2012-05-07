#include "tia_client_based_libtia.h"

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

    //TODO: crate method startDataConnection in tia_client_ instead
    tia_client_.startReceiving(udp_data_connection);
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

}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::startReceiving()
{
    //TODO: call start receiving of tia_client that only sends the start receiving command
}

//-----------------------------------------------------------------------------

void TiAQtClientBasedLibTiA::stopReceiving()
{
    tia_client_.stopReceiving();
}
