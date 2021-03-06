#include "fourier_transform_thread.h"

#include <QDebug>
#include <QSettings>

#include <cmath>

namespace TiAScope {

namespace FourierTransformThreadHelper
{
    void allocateBuffers (FFTReal::flt_t*& in, FFTReal::flt_t*& out, unsigned& size, unsigned num_samples);
}

//-------------------------------------------------------------------------------------------------
FourierTransformThread::FourierTransformThread (QSharedPointer<DataBuffer const> data_buffer, QObject *parent) :
    QThread(parent),
    data_buffer_ (data_buffer),
    ft_data_ (new QVector<double>()),
    in_buffer_ (0),
    out_buffer_ (0),
    buffers_size_ (0),
    running_ (false),
    ln_ (false),
    window_size_in_secs_ (2)
{
    setObjectName (QString("FourierTransformThread - 0x").append(QString::number(reinterpret_cast<int64_t const>(this), 16)));
}

//-------------------------------------------------------------------------------------------------
void FourierTransformThread::enableFT (SignalTypeFlag signal, int channel, bool enabled)
{
    ft_enabled_[signal][channel] = enabled;
    Q_EMIT FTEnabledChanged (signal, channel, enabled);
}

//-------------------------------------------------------------------------------------------------
void FourierTransformThread::run ()
{
    time_.restart ();
    int elapsed_time = 0;
    running_ = true;
    int update_interval = 0;
    //QSettings settings;

    while (running_)
    {
        //ln_ = settings.value ("fourier/ln", ln_).toBool();
        //window_size_in_secs_ = settings.value ("fourier/window_size_s", window_size_in_secs_).toDouble();
        Q_FOREACH (SignalTypeFlag signal, ft_enabled_.keys())
        {
            Q_FOREACH (int channel, ft_enabled_[signal].keys())
            {
                if (ft_enabled_[signal][channel])
                    calculateAndEmitFT (signal, channel);
                else
                {
                    Q_EMIT FTFinished (QVector<double> (), signal, channel, 0);
                }
            }
        }

        elapsed_time = time_.elapsed ();
        update_interval = 80;//settings.value ("fourier/update_interval_msec", 160).toInt();
        if (elapsed_time < update_interval)
            usleep ((update_interval - elapsed_time) * 1000);
        time_.restart ();
    }
}


//-------------------------------------------------------------------------------------------------
void FourierTransformThread::calculateAndEmitFT (SignalTypeFlag signal, int channel)
{
    int sample_rate = data_buffer_->sampleRate (signal);
    int buffer_limit = data_buffer_->getSampleLimit (signal);

    int num_samples = 1; // has to become a power of 2
    while (num_samples < (sample_rate * window_size_in_secs_))
        num_samples *= 2;

    FourierTransformThreadHelper::allocateBuffers (in_buffer_, out_buffer_, buffers_size_, num_samples);

    data_buffer_->lockForRead();
    for (int x = 0; x < num_samples; x++)
    {
        if (x < (sample_rate * 2))
            in_buffer_[x] = data_buffer_->getData (signal, channel, buffer_limit - x);
        else
            in_buffer_[x] = 0;
    }

    data_buffer_->unlockForRead();

    FFTReal fft_object (num_samples);
    fft_object.do_fft (out_buffer_, in_buffer_);
    fft_object.rescale (out_buffer_);

    ft_data_->resize (num_samples / 2);
    for (int index = 0; index < (num_samples / 2) ; index++)
        ft_data_->operator [](index) = pow(out_buffer_[index], 2) + pow(out_buffer_[(num_samples/2)+index], 2);
    if (ln_)
    {
        for (int index = 0; index < ft_data_->size(); index++)
        {
            if (ft_data_->operator [](index) > 0)
                ft_data_->operator [](index) = log (ft_data_->operator [](index));
        }
    }

    Q_EMIT FTFinished (*ft_data_, signal, channel, sample_rate / 2);
}

//-------------------------------------------------------------------------------------------------
namespace FourierTransformThreadHelper
{
    void allocateBuffers (FFTReal::flt_t*& in, FFTReal::flt_t*& out, unsigned& size, unsigned num_samples)
    {
        if (num_samples > size)
        {
            if (in)
                delete[] in;
            if (out)
                delete[] out;
            size = num_samples;
            in = new FFTReal::flt_t [size];
            out = new FFTReal::flt_t [size];
        }
    }
}


} // TiAScope
