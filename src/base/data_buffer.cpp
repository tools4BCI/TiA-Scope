#include "data_buffer.h"

#include <QWriteLocker>
#include <QReadLocker>

#include <iostream>
#include <cmath>

namespace TiAScope {

//-----------------------------------------------------------------------------
DataBuffer::DataBuffer (tobiss::SignalInfo::SignalMap const& signal_map, int buffer_size_in_seconds)
{
    setObjectName ("DataBuffer");

    for (tobiss::SignalInfo::SignalMap::const_iterator signal_iter = signal_map.begin();
         signal_iter != signal_map.end();
         ++signal_iter)
    {
        SignalTypeFlag signal_type = TypeConverter::stdStringToSignalTypeFlag (signal_iter->first);
        sample_limit_[signal_type] = buffer_size_in_seconds * signal_iter->second.samplingRate ();
        sampling_rate_[signal_type] = signal_iter->second.samplingRate ();
        for (unsigned channel_index = 0; channel_index < signal_iter->second.channels ().size ();
             ++channel_index)
        {
            data_[signal_type][channel_index] = QVector<double> (sample_limit_[signal_type], 0);
            end_index_[signal_type][channel_index] = 0;
            number_new_samples_[signal_type][channel_index] = 0;
        }
    }
}

//-----------------------------------------------------------------------------
void DataBuffer::appendData (SignalTypeFlag signal, int channel, QList<double> const& data)
{
    QWriteLocker write_locker (&lock_);

    number_new_samples_[signal][channel] += data.size();
    QVector<double>& channel_data = data_[signal][channel];
    int& end_index = end_index_[signal][channel];
    Q_FOREACH (double sample, data)
    {
        channel_data[end_index] = sample;
        end_index++;
        if (end_index >= channel_data.size ())
            end_index = 0;
    }
}

//-------------------------------------------------------------------------
void DataBuffer::setAperiodicValues (boost::uint32_t signal_flag, boost::uint32_t device_id, QList<double> const& data)
{
    aperiodic_data_[signal_flag][device_id] = data;
}

//-------------------------------------------------------------------------
QList<DeviceID> DataBuffer::getAperiodicDeviceIDs (boost::uint32_t signal_flag) const
{
    return aperiodic_data_[signal_flag].keys ();
}

//-------------------------------------------------------------------------
QList<double> DataBuffer::getAperiodicValues (boost::uint32_t signal_flag, DeviceID device_id) const
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
