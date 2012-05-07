#include "tia_datapacket_basedlibtia.h"
#include <stdexcept>

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------

DataPacketBasedLibTiA::DataPacketBasedLibTiA(tia::DataPacket &tia_packet)
    : tia_packet_(tia_packet)
{
}

//-----------------------------------------------------------------------------

DataPacketBasedLibTiA::~DataPacketBasedLibTiA ()
{
    // nothing to do here
}

//-----------------------------------------------------------------------------

QVector<double> DataPacketBasedLibTiA::getData (SignalTypeFlag signal, ChannelIndex channel) const
{
    std::vector<double> all_chan_values = tia_packet_.getSingleDataBlock(signal);

    quint32 nr_chans = tia_packet_.getNrOfChannels(signal);

    if(nr_chans <= channel)
        throw std::invalid_argument("channel is greater as available channels");

    quint32 block_size = tia_packet_.getNrSamplesPerChannel(signal);

    std::vector<double>::iterator position = all_chan_values.begin() + channel * block_size;

//    std::cout << "block_size: " << block_size << std::endl;
//    std::cout << "nr_chans: " << nr_chans << std::endl;

    std::vector<double>::iterator end_pos = position + block_size;

    std::vector<double> data;

    data.assign(position,end_pos);

    return QVector<double>::fromStdVector(data);
}

//-----------------------------------------------------------------------------

QSet<SignalTypeFlag> DataPacketBasedLibTiA::getSignals () const
{
    quint32 flags = tia_packet_.getFlags();

    QSet<SignalTypeFlag> signal_set;

    signal_set.clear();

    for (SignalTypeFlag signal_flag_iter = 0x01;
         signal_flag_iter < (quint32)(0x01 << (sizeof(SignalTypeFlag) * 8 - 1));
         signal_flag_iter <<= 1)
    {
        if (flags & signal_flag_iter)
        {
            signal_set.insert(signal_flag_iter);
        }
    }

    return signal_set;
}

//-----------------------------------------------------------------------------

ChannelIndex DataPacketBasedLibTiA::getNumChannels (SignalTypeFlag signal) const
{
    return tia_packet_.getNrOfChannels(signal);
}

}


