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

    unsigned num_signals = 0;

    for (SignalTypeFlag signal_flag_iter = 0x1;
         signal_flag_iter < 0x200000;
         signal_flag_iter <<= 1)
    {
        if (flags & signal_flag_iter)
            num_signals++;
    }
    if (bytes.size () < (28 + (4 * num_signals)))
        return false;

    unsigned num_values = 0;
    for (int signal_index = 0; signal_index < num_signals; signal_index++)
    {
        quint16 values = 0;
        memcpy (&values, bytes.mid (28 + (2 * signal_index), 2).constData(), 2);
        num_values += values;
    }

    unsigned needed = (num_values * 4) + (28 + (4 * num_signals));
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

    unsigned num_signals = 0;

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
        for (int channel_index = 0; channel_index < num_channels; channel_index++)
        {
            for (int value_index = 0; value_index < block_sizes[signal_index]; value_index++)
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
DataPacketVersion2::DataPacketVersion2 (QTcpSocket& data_stream)
{
//    //data_stream.setByteOrder (QDataStream::LittleEndian);
//    //data_stream.waitForReadyRead ();
//    quint32 flags = 0;
//    waitForBytes (4, data_stream);
//    //data_stream.device()->waitForReadyRead (30000);
//    //qDebug () << "Datapacket available bytes: " <<    data_stream.device()->bytesAvailable();
//    flags = data_from_socket_.right(4).toUInt();
//    data_from_socket_.remove (0, 4);
////    if (4 != data_stream.read (reinterpret_cast<char*>(&flags), 4))
////    {
////        qDebug () << "error while reading!!!!!!!!!! flags";
////    }

//    //data_stream >> flags;
//    //qDebug () << "Datapacket flags: " << flags;
//    quint32 blub_flag = flags;
//    if ((blub_flag & VERSION_FLAG_) != VERSION_FLAG_)
//    {
//        flags *= 1;
//        throw TiAException ("DataPacketVersion2: Version flag does not match!");
//    }

//    for (SignalTypeFlag signal_flag_iter = SMALLEST_SIGNAL_FLAG_;
//         signal_flag_iter < HIGHEST_SIGNAL_FLAG_;
//         signal_flag_iter <<= 1)
//    {
//        if (flags & signal_flag_iter)
//        {
//            signals_.append (signal_flag_iter);
//            data_.insert (signal_flag_iter, QMap<ChannelIndex, QVector<double> > ());
//        }
//    }
//    waitForBytes (24, data_stream);

//    quint64 packet_nr = data_from_socket_.right(8).toULongLong();
//    data_from_socket_.remove (0, 8);
//    //    if (8 != data_stream.read (reinterpret_cast<char*>(&packet_nr), 8))
////    {
////        qDebug () << "error while reading!!!!!!!!!! packet nr";
////    }
//    // qDebug () << "flags = " << flags << " packet nr = " << packet_nr;

//    if (16 != data_stream.read (16).size())
//    {
//        qDebug () << "error while reading!!!!!!!!!! skipping";
//    }
////    data_stream.skipRawData (8); // skip paket number
////    data_stream.skipRawData (8); // skip connection paket number
////    data_stream.skipRawData (8); // skip timestamp

//    //qDebug () << "num signals: " << signals_.size();

//    waitForBytes (signals_.size () * 4, data_stream);

//    QVector<quint16> num_values;
//    quint32 num_values_ac = 0;
//    for (int signal_index = 0; signal_index < signals_.size(); signal_index++)
//    {
//        quint16 num_value;
//        // data_stream >> num_value;
//        num_value = data_from_socket_.right(2).toUShort();
//        data_from_socket_.remove (0, 2);
////        if (2 != data_stream.read (reinterpret_cast<char*>(&num_value), 2))
////        {
////            qDebug () << "error while reading!!!!!!!!!! num values";
////        }
//        num_values.append (num_value);
//        num_values_ac += num_value;
//    }
//    QVector<quint16> block_sizes;
//    for (int signal_index = 0; signal_index < signals_.size(); signal_index++)
//    {
//        quint16 block_size;
//        // data_stream >> block_size;
//        block_size = data_from_socket_.right(2).toUShort();
//        data_from_socket_.remove (0, 2);
////        if (2 != data_stream.read (reinterpret_cast<char*>(&block_size), 2))
////        {
////            qDebug () << "error while reading!!!!!!!!!! block sizes";
////        }

//        block_sizes.append (block_size);
//    }

//    if (num_values_ac * 4 > 36732)
//    {
//        num_values_ac *= 1;
//        qDebug () << "UAAAAAAAAAAAAAAAAA";
//    }

//    quint32 values_left_ac = num_values_ac;

//    for (int signal_index = 0; signal_index < signals_.size(); signal_index++)
//    {
//        ChannelIndex num_channels = num_values[signal_index] / block_sizes[signal_index];
//        for (int channel_index = 0; channel_index < num_channels; channel_index++)
//        {
//            for (int value_index = 0; value_index < block_sizes[signal_index]; value_index++)
//            {
//                float value;
//                waitForBytes (4, data_stream);
//                if (4 != data_stream.read (reinterpret_cast<char*>(&value), 4))
//                {
//                    qDebug () << "error while reading!!!!!!!!!! values";
//                }
//                // data_stream.readBytes ();
//                //data_stream.skipRawData (4);
//                //data_stream >> value;
//                data_[signals_[signal_index]][channel_index].append (value);
//                values_left_ac--;
//            }
//        }
//    }
//    //qDebug () << "num signals: " << signals_.size() << "; values left: " << values_left_ac << "; num values ac = " << num_values_ac;
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

//-----------------------------------------------------------------------------
void DataPacketVersion2::waitForBytes (unsigned num_bytes, QTcpSocket& data_stream)
{
    while (num_bytes < data_from_socket_.size ())
    {
        data_from_socket_.append (data_stream.readAll());
        QThread::yieldCurrentThread ();
    }
    //data_stream.waitForReadyRead ();
//    return;
    //if (num_bytes == 0)
    //    return;
    //data_stream.device()->waitForReadyRead (30000);
//    qint64 available = data_stream.bytesAvailable();
//    while (num_bytes > available)
//    {
//        data_stream.waitForReadyRead ();
//        //QThread::yieldCurrentThread ();
//        //data_stream.device()->waitForReadyRead (30000);
//        //qDebug () << "expecting " << num_bytes << "bytes, available " << available;
//        available = data_stream.bytesAvailable();
//    }

}

}
