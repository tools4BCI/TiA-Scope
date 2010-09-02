#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "config/ss_meta_info.h"

#include <QObject>
#include <QHash>
#include <QList>
#include <QString>
#include <QReadWriteLock>
#include <QVector>
#include <QVarLengthArray>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------
class DataBuffer : public QObject
{
    Q_OBJECT
public:
    //-------------------------------------------------------------------------
    explicit DataBuffer (SignalInfo::SignalMap const& signal_map, int buffer_size_in_seconds);

    //-------------------------------------------------------------------------
    /// deletes the oldest data if the buffer size is exceeded
    void appendData (QString const& signal, int channel, QList<double> const& data);

    //-------------------------------------------------------------------------
    void lockForRead () const {lock_.lockForRead();}

    //-------------------------------------------------------------------------
    double getData (QString const& signal, int channel, int sample_index) const;

    //-------------------------------------------------------------------------
    /// fill the data_array with the last samples of the given channel
    void getData (QString const& signal, int channel, QVarLengthArray<double>& data_array) const;

    //-------------------------------------------------------------------------
    int numberNewSamples (QString const& signal, int channel) const;

    //-------------------------------------------------------------------------
    void unlockForRead () const {lock_.unlock();}

    //-------------------------------------------------------------------------
    int sampleRate (QString const& signal) const;

    //-------------------------------------------------------------------------
    int getSampleLimit (QString const& signal) const;
private:
    QHash<QString, QHash<int, QVector<double> > > data_;
    QHash<QString, QHash<int, int> > end_index_;
    mutable QHash<QString, QHash<int, int> > number_new_samples_;
    QHash<QString, int> sample_limit_;
    QHash<QString, int> sampling_rate_;
    mutable QReadWriteLock lock_;
};

} } // namespace

#endif // DATA_BUFFER_H
