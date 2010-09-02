#include "data_buffer.h"

#include <QWriteLocker>
#include <QReadLocker>

#include <iostream>
#include <cmath>

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
DataBuffer::DataBuffer (SignalInfo::SignalMap const& signal_map, int buffer_size_in_seconds)
{
    for (SignalInfo::SignalMap::const_iterator signal_iter = signal_map.begin();
         signal_iter != signal_map.end();
         ++signal_iter)
    {
        QString signal_name (signal_iter->first.c_str());
        sample_limit_[signal_name] = buffer_size_in_seconds * signal_iter->second.samplingRate ();
        sampling_rate_[signal_name] = signal_iter->second.samplingRate ();
        for (unsigned channel_index = 0; channel_index < signal_iter->second.channels ().size ();
             ++channel_index)
        {
            data_[signal_name][channel_index] = QVector<double> (sample_limit_[signal_iter->first.c_str()], 0);
            end_index_[signal_name][channel_index] = 0;
            number_new_samples_[signal_name][channel_index] = 0;
        }
    }
}

//-----------------------------------------------------------------------------
void DataBuffer::appendData (QString const& signal, int channel, QList<double> const& data)
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

//-----------------------------------------------------------------------------
double DataBuffer::getData (QString const& signal, int channel, int sample_index) const
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
void DataBuffer::getData (QString const& signal, int channel, QVarLengthArray<double>& data_array) const
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
int DataBuffer::numberNewSamples (QString const& signal, int channel) const
{
    int new_samples = number_new_samples_[signal][channel];
    number_new_samples_[signal][channel] = 0;
    return new_samples;
}


//-------------------------------------------------------------------------
int DataBuffer::sampleRate (QString const& signal) const
{
    return sampling_rate_ [signal];
}

//-----------------------------------------------------------------------------
int DataBuffer::getSampleLimit (QString const& signal) const
{
    return sample_limit_[signal];
}

} } // namespace
