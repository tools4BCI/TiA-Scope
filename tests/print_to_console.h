#ifndef PRINT_TO_CONSOLE_H
#define PRINT_TO_CONSOLE_H

#include "../../src/data_collector/qt_tia_client/datapacket.h"


#include "tia/defines.h"
#include "tia/constants.h"

using namespace TiAQtImplementation;

void printDataPacketToConsole(QSharedPointer<DataPacket> packet)
{
    tia::Constants flag_string_converter;

    foreach (SignalTypeFlag signal, packet->getSignals())
    {
        quint32 chan_count = packet->getNumChannels(signal);

        QVector<double> signal_samples;

        for (quint32 chan = 0; chan < chan_count; ++chan) {
            QVector<double> samples = packet->getData(signal,chan);

            for (QVector<double>::iterator it = samples.begin(); it != samples.end(); ++it) {
                signal_samples.append(*it);
            }

        }

        qDebug() << "signal: " << flag_string_converter.getSignalName(signal).c_str() << ", channels: " << chan_count << ", data: " << signal_samples;

    }
}

#endif // PRINT_TO_CONSOLE_H
