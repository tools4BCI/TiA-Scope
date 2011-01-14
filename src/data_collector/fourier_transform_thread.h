#ifndef FOURIER_TRANSFORM_THREAD_H
#define FOURIER_TRANSFORM_THREAD_H

#include "base/data_buffer.h"
#include "base/FFTReal.h"

#include <QThread>
#include <QTime>
#include <QString>
#include <QMutex>
#include <QMutexLocker>

namespace TiAScope {


class FourierTransformThread : public QThread
{
    Q_OBJECT
public:
    explicit FourierTransformThread (QSharedPointer<DataBuffer const> data_buffer,
                                     QObject *parent = 0);

public Q_SLOTS:
    void enableFT (SignalTypeFlag signal, int channel, bool enabled);

    void stop () {running_ = false;}

Q_SIGNALS:
    void FTFinished (QVector<double> ft, SignalTypeFlag signal, int channel, int frequency_range);
    void FTEnabledChanged (SignalTypeFlag signal, int channel, bool enbaled);

private:
    virtual void run ();

    void calculateAndEmitFT (SignalTypeFlag signal, int channel);

    QSharedPointer<DataBuffer const> data_buffer_;
    QSharedPointer<QVector<double> > ft_data_;
    FFTReal::flt_t* in_buffer_;
    FFTReal::flt_t* out_buffer_;
    unsigned buffers_size_;

    QTime time_;
    QMutex ft_enabled_mutex_;
    QMap<SignalTypeFlag, QHash<int, bool> > ft_enabled_;
    bool running_;
    bool ln_;
    double window_size_in_secs_;
};

} // TiAScope

#endif // FOURIER_TRANSFORM_THREAD_H
