#include "data_buffer.h"
#include "data_collector/filters.h"

#include "tia/constants.h"

#include <QWriteLocker>
#include <QReadLocker>
#include <QDebug>

#include <iostream>
#include <cmath>

using namespace TiAQtImplementation;

namespace TiAScope {

//-----------------------------------------------------------------------------
DataBuffer::DataBuffer (TiAQtImplementation::TiAMetaInfo const& meta_info, int buffer_size_in_seconds)
{
    setObjectName ("DataBuffer");

    Q_FOREACH (SignalTypeFlag signal_flag, meta_info.getSignalTypes ())
    {
        sample_limit_[signal_flag] = buffer_size_in_seconds * meta_info.getSamplingRate (signal_flag);
        sampling_rate_[signal_flag] = meta_info.getSamplingRate (signal_flag);
        for (ChannelIndex channel_index = 0; channel_index < meta_info.getNumChannels (signal_flag);
             ++channel_index)
        {
            data_[signal_flag][channel_index] = QVector<double> (sample_limit_[signal_flag], 0);
            end_index_[signal_flag][channel_index] = 0;
            number_new_samples_[signal_flag][channel_index] = 0;
            filter_ids_[signal_flag][channel_index] = Filters::instance().registerSignalToBeFiltered (sampling_rate_[signal_flag]);
        }
    }
}

//-----------------------------------------------------------------------------
//bad hack to undefine signals that is used by Qt
//but at the same time defines a method of SignalInfo
//used in libTiA
#undef signals

DataBuffer::DataBuffer (const tia::SSConfig &meta_info, int buffer_size_in_seconds)
{
    setObjectName("DataBuffer");

    tia::Constants tia_constatnts;

    for (tia::SignalInfo::SignalMap::const_iterator signal_iter = meta_info.signal_info.signals().begin ();
         signal_iter != meta_info.signal_info.signals ().end (); ++signal_iter)
    {
        quint32 signal_flag = tia_constatnts.getSignalFlag( signal_iter->second.type());

        sample_limit_[signal_flag] = buffer_size_in_seconds * signal_iter->second.samplingRate();
        sampling_rate_[signal_flag] = signal_iter->second.samplingRate();

        for (unsigned channel_index = 0; channel_index < signal_iter->second.channels().size(); channel_index++)
        {
            data_[signal_flag][channel_index] = QVector<double> (sample_limit_[signal_flag], 0);
            end_index_[signal_flag][channel_index] = 0;
            number_new_samples_[signal_flag][channel_index] = 0;
            filter_ids_[signal_flag][channel_index] = Filters::instance().registerSignalToBeFiltered (sampling_rate_[signal_flag]);

        }

    }
}

//revert hack that signals is undefined
#define signals Q_SIGNALS

//-----------------------------------------------------------------------------
void DataBuffer::appendData (SignalTypeFlag signal, int channel, QList<double> const& data)
{
    QWriteLocker write_locker (&lock_);

    number_new_samples_[signal][channel] += data.size();
    QVector<double>& channel_data = data_[signal][channel];
    int& end_index = end_index_[signal][channel];
    Q_FOREACH (double sample, data)
    {
        channel_data[end_index] = Filters::instance().clock (filter_ids_[signal][channel], sample);
        min_[signal][channel] = qMin (min_[signal][channel], channel_data[end_index]);
        max_[signal][channel] = qMax (max_[signal][channel], channel_data[end_index]);
        end_index++;
        if (end_index >= channel_data.size ())
            end_index = 0;
    }
}

//-------------------------------------------------------------------------
void DataBuffer::setAperiodicValues (quint32 signal_flag, quint32 device_id, QList<double> const& data)
{
    aperiodic_data_[signal_flag][device_id] = data;
}

//-------------------------------------------------------------------------
QList<DeviceID> DataBuffer::getAperiodicDeviceIDs (SignalTypeFlag signal_flag) const
{
    return aperiodic_data_[signal_flag].keys ();
}

//-------------------------------------------------------------------------
QList<double> DataBuffer::getAperiodicValues (SignalTypeFlag signal_flag, DeviceID device_id) const
{
    return aperiodic_data_[signal_flag][device_id];
}

//-----------------------------------------------------------------------------
double DataBuffer::getData (SignalTypeFlag signal, int channel, int sample_index) const
{
    int const limit = sample_limit_[signal];
    int const end_index = end_index_[signal][channel];

    if (sample_index < 0)
        return 0;
    if (sample_index >= limit)
        return 0;

    sample_index += end_index;

    if (sample_index >= limit)
        sample_index -= limit;
    double return_value = data_[signal][channel][sample_index];
    return return_value;
}

//-------------------------------------------------------------------------
void DataBuffer::getData (SignalTypeFlag signal, int channel, QVarLengthArray<double>& data_array) const
{
    if (!sample_limit_.contains (signal) ||
        !end_index_.contains (signal))
        return;
    if (!end_index_[signal].contains (channel))
        return;

    int const limit = sample_limit_[signal];
    int const end_index = end_index_[signal][channel];

    int sample_index = limit - data_array.size();
    for (int index = 0; index < data_array.size(); index++)
    {
        if (sample_index < 0 || sample_index >= limit)
            data_array[index] = 0;
        else
        {
            int look_up_sample_index = sample_index + end_index;
            if (look_up_sample_index >= limit)
                look_up_sample_index -= limit;
            data_array[index] = data_[signal][channel][look_up_sample_index];
        }
        sample_index++;
    }
}

//-------------------------------------------------------------------------
double DataBuffer::getMin (TiAQtImplementation::SignalTypeFlag signal, int channel) const
{
    if (!min_.contains(signal))
        return 0;
    if (min_[signal].size() <= channel)
        return 0;
    return min_[signal][channel];
}

//-------------------------------------------------------------------------
double DataBuffer::getMax (TiAQtImplementation::SignalTypeFlag signal, int channel) const
{
    if (!max_.contains(signal))
        return 0;
    if (max_[signal].size() <= channel)
        return 0;
    return max_[signal][channel];
}

//-------------------------------------------------------------------------
int DataBuffer::numberNewSamples (SignalTypeFlag signal, int channel) const
{
    int new_samples = number_new_samples_[signal][channel];
    number_new_samples_[signal][channel] = 0;
    return new_samples;
}


//-------------------------------------------------------------------------
int DataBuffer::sampleRate (SignalTypeFlag signal) const
{
    return sampling_rate_ [signal];
}

//-----------------------------------------------------------------------------
int DataBuffer::getSampleLimit (SignalTypeFlag signal) const
{
    return sample_limit_[signal];
}

} // TiAScope
