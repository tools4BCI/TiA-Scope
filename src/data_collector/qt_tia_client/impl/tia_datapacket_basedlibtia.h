#ifndef TIA_DATAPACKET_BASEDLIBTIA_H
#define TIA_DATAPACKET_BASEDLIBTIA_H


#include "../datapacket.h"
#include "tia/data_packet_interface.h"

#include <QList>
#include <QMap>
#include <QDataStream>
#include <QTcpSocket>

namespace TiAQtImplementation
{


class DataPacketBasedLibTiA : public DataPacket
{
public:
    DataPacketBasedLibTiA(tia::DataPacket &tia_packet);

    virtual ~DataPacketBasedLibTiA ();
    virtual QVector<double> getData (SignalTypeFlag signal, quint32 channel) const;
    virtual QSet<SignalTypeFlag> getSignals () const;
    virtual quint32 getNumChannels (SignalTypeFlag signal) const;

private:

    tia::DataPacket &tia_packet_;
};


}

#endif // TIA_DATAPACKET_BASEDLIBTIA_H
