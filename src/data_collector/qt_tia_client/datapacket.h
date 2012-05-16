#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "tia/defines.h"
#include <QVector>
#include <QSet>

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
class DataPacket
{
public:
    virtual ~DataPacket () {}
    virtual QVector<double> getData (SignalTypeFlag signal, quint32 channel) const = 0;
    virtual QSet<SignalTypeFlag> getSignals () const = 0;
    virtual quint32 getNumChannels (SignalTypeFlag signal) const = 0;
};

}

#endif // DATAPACKET_H
