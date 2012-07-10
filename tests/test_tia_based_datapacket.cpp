#include <QtCore/QString>


#include "UnitTest++.h"


#include "data_collector/qt_tia_client/impl/tia_datapacket_basedlibtia.h"
#include "data_collector/qt_tia_client/impl/tia_datapacket_version2.h"

#include "tia/tia_client.h"

#include "tia/defines.h"
#include "tia/constants.h"

using namespace TiAQtImplementation;

//-----------------------------------------------------------------------------

//TEST(toUpper)

//{
//    QString str = "Hello";

//    CHECK(str.toUpper() == QString("HELLO"));
//}

//-----------------------------------------------------------------------------

TEST(emptyDataPacket)
{
    tia::TiAClient client(true);

    tia::DataPacket *empty_tia_packet = client.getEmptyDataPacket();

    DataPacketBasedLibTiA empty_packet (*empty_tia_packet);

    CHECK_EQUAL(empty_packet.getSignals().count(),0);

    CHECK_EQUAL(empty_packet.getNumChannels(SIG_EMG), 0);

}

//-----------------------------------------------------------------------------

TEST(filledDataPacket)
{
    tia::TiAClient client(true);

    tia::DataPacket *tia_packet = client.getEmptyDataPacket();


    tia_packet->reset();

    std::vector<double> raw_data;

    quint32 raw_data_size = 100;

    for (quint32 counter = 0; counter < raw_data_size; ++counter)
    {
        raw_data.push_back((double)rand()/(double)RAND_MAX);
    }

    quint32 block_sizes [] = { 1, 2};

    tia_packet->insertDataBlock(raw_data,SIG_EEG,block_sizes[0]);
    tia_packet->insertDataBlock(raw_data,SIG_EOG,block_sizes[1]);

    DataPacketBasedLibTiA packet (*tia_packet);


    CHECK_EQUAL(packet.getSignals().count(),2);

    CHECK_EQUAL(packet.getNumChannels(SIG_EEG), raw_data_size / block_sizes[0]);
    CHECK_EQUAL(packet.getNumChannels(SIG_EOG), raw_data_size / block_sizes[1]);


    CHECK_EQUAL(packet.getData(SIG_EEG,0).count(),1);
    CHECK_EQUAL(packet.getData(SIG_EOG,0).count(),2);

    tia::Constants flag_string_converter;

    foreach (SignalTypeFlag signal, packet.getSignals())
    {
        quint32 chan_count = packet.getNumChannels(signal);

        QVector<double> signal_samples;

        for (quint32 chan = 0; chan < chan_count; ++chan) {
            QVector<double> samples = packet.getData(signal,chan);

            for (QVector<double>::iterator it = samples.begin(); it != samples.end(); ++it) {
                signal_samples.append(*it);
            }

        }

//        qDebug() << "signal: " << flag_string_converter.getSignalName(signal).c_str() << ", channels: " << chan_count << ", data: " << signal_samples;

        CHECK_ARRAY_CLOSE(signal_samples.toStdVector(),raw_data,raw_data_size,0.00000);


    }

}

