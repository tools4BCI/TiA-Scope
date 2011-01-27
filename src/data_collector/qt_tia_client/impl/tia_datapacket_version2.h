#ifndef TIA_DATAPACKET_VERSION2_H
#define TIA_DATAPACKET_VERSION2_H

#include "../datapacket.h"

#include <QList>
#include <QMap>
#include <QDataStream>
#include <QTcpSocket>

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
class DataPacketVersion2 : public DataPacket
{
public:
    DataPacketVersion2 (QByteArray bytes);
    virtual ~DataPacketVersion2 ();
    virtual QVector<double> getData (SignalTypeFlag signal, ChannelIndex channel) const;
    virtual QSet<SignalTypeFlag> getSignals () const;
    virtual ChannelIndex getNumChannels (SignalTypeFlag signal) const;

    static unsigned canCreate (QByteArray bytes);

private:
    static quint32 const VERSION_FLAG_;
    static quint32 const SIGNAL_FLAG_MASK_;
    static quint32 const SMALLEST_SIGNAL_FLAG_;
    static quint32 const HIGHEST_SIGNAL_FLAG_;
    QVector<SignalTypeFlag> signals_;
    QMap<SignalTypeFlag, QMap<ChannelIndex, QVector<double> > >data_;
    QByteArray data_from_socket_;
};

}

#endif // TIA_DATAPACKET_VERSION2_H
