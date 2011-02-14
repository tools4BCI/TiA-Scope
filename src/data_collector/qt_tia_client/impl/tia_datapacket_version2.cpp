#include "tia_datapacket_version2.h"
#include "../tia_exception.h"

#include <QThread>
#include <QDebug>

namespace TiAQtImplementation
{

quint32 const DataPacketVersion2::VERSION_FLAG_ = 0x0A400000;
quint32 const DataPacketVersion2::SIGNAL_FLAG_MASK_ = 0x1FFFFF;
quint32 const DataPacketVersion2::SMALLEST_SIGNAL_FLAG_ = 0x1;
quint32 const DataPacketVersion2::HIGHEST_SIGNAL_FLAG_ = 0x200000;

//-----------------------------------------------------------------------------
unsigned DataPacketVersion2::canCreate (QByteArray bytes)
{
    if (bytes.size() < 4)
        return false;
    quint32 flags = 0;
    memcpy (&flags, bytes.left(4).constData(), 4);

    int num_signals = 0;

    for (SignalTypeFlag signal_flag_iter = 0x1;
         signal_flag_iter < 0x200000;
         signal_flag_iter <<= 1)
    {
        if (flags & signal_flag_iter)
            num_signals++;
    }
    if (bytes.size () < (28 + (4 * num_signals)))
        return false;

    int num_values = 0;
    for (int signal_index = 0; signal_index < num_signals; signal_index++)
    {
        quint16 values = 0;
        memcpy (&values, bytes.mid (28 + (2 * signal_index), 2).constData(), 2);
        num_values += values;
    }

    int needed = (num_values * 4) + (28 + (4 * num_signals));
    if (bytes.size() < needed)
        return 0;
    else
        return needed;
}

//-----------------------------------------------------------------------------
DataPacketVersion2::DataPacketVersion2 (QByteArray bytes)
{
    if (bytes.size() < 4)
        throw TiAException ("DataPacketVersion2: bytes less than 4!");

    quint32 flags = 0;
    memcpy (&flags, bytes.left(4).constData(), 4);

    if ((flags & VERSION_FLAG_) != VERSION_FLAG_)
        throw TiAException ("DataPacketVersion2: Version flag does not match!");

    int num_signals = 0;

    for (SignalTypeFlag signal_flag_iter = SMALLEST_SIGNAL_FLAG_;
         signal_flag_iter < HIGHEST_SIGNAL_FLAG_;
         signal_flag_iter <<= 1)
    {
        if (flags & signal_flag_iter)
        {
            signals_.append (signal_flag_iter);
            num_signals++;
        }
    }
    if (bytes.size () < (28 + (4 * num_signals)))
        throw TiAException ("DataPacketVersion2: to less bytes for nr values and blocks!");

    QVector<quint16> num_values;
    for (int signal_index = 0; signal_index < signals_.size(); signal_index++)
    {
        quint16 num_value = 0;
        memcpy (&num_value, bytes.mid (28 + (2 * signal_index), 2).constData(), 2);
        num_values.append (num_value);
    }
    QVector<quint16> block_sizes;
    for (int signal_index = 0; signal_index < signals_.size(); signal_index++)
    {
        quint16 block_size = 0;
        memcpy (&block_size, bytes.mid (28 + (signals_.size() * 2) + (2 * signal_index), 2).constData(), 2);
        block_sizes.append (block_size);
    }

    unsigned offset = 28 + (signals_.size() * 4);
    for (int signal_index = 0; signal_index < signals_.size(); signal_index++)
    {
        ChannelIndex num_channels = num_values[signal_index] / block_sizes[signal_index];
        for (unsigned channel_index = 0; channel_index < num_channels; channel_index++)
        {
            for (unsigned value_index = 0; value_index < block_sizes[signal_index]; value_index++)
            {
                float value;
                memcpy (&value, bytes.mid (offset, 4).constData(), 4);
                data_[signals_[signal_index]][channel_index].append (value);
                offset += 4;
            }
        }
    }
}

//-----------------------------------------------------------------------------
DataPacketVersion2::~DataPacketVersion2 ()
{
    // nothing to do here
}

//-----------------------------------------------------------------------------
QVector<double> DataPacketVersion2::getData (SignalTypeFlag signal, ChannelIndex channel) const
{
    return data_[signal][channel];
}

//-----------------------------------------------------------------------------
QSet<SignalTypeFlag> DataPacketVersion2::getSignals () const
{
    return signals_.toList().toSet ();
}

//-----------------------------------------------------------------------------
ChannelIndex DataPacketVersion2::getNumChannels (SignalTypeFlag signal) const
{
    return data_[signal].size ();
}

}
