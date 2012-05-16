#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "user_types.h"

#include "tia/defines.h"
#include "tia/ssconfig.h"

#include <QObject>
#include <QMap>
#include <QHash>
#include <QList>
#include <QString>
#include <QReadWriteLock>
#include <QVector>
#include <QVarLengthArray>


namespace TiAScope {

//-------------------------------------------------------------------------
class DataBuffer : public QObject
{
    Q_OBJECT
public:
    //-------------------------------------------------------------------------
    explicit DataBuffer (tia::SSConfig const& meta_info, int buffer_size_in_seconds);

    //-------------------------------------------------------------------------
    /// deletes the oldest data if the buffer size is exceeded
    void appendData (SignalTypeFlag signal, int channel, QList<double> const& data);

    //-------------------------------------------------------------------------
    void setAperiodicValues (SignalTypeFlag signal_flag, DeviceID device_id, QList<double> const& data);

    //-------------------------------------------------------------------------
    QList<DeviceID> getAperiodicDeviceIDs (SignalTypeFlag signal_flag) const;

    //-------------------------------------------------------------------------
    QList<double> getAperiodicValues (SignalTypeFlag signal_flag, DeviceID device_id) const;

    //-------------------------------------------------------------------------
    void lockForRead () const {lock_.lockForRead();}

    //-------------------------------------------------------------------------
    double getData (SignalTypeFlag signal, int channel, int sample_index) const;

    //-------------------------------------------------------------------------
    double getMin (SignalTypeFlag signal, int channel) const;

    //-------------------------------------------------------------------------
    double getMax (SignalTypeFlag signal, int channel) const;

    //-------------------------------------------------------------------------
    /// fill the data_array with the last samples of the given channel
    void getData (SignalTypeFlag signal, int channel, QVarLengthArray<double>& data_array) const;

    //-------------------------------------------------------------------------
    int numberNewSamples (SignalTypeFlag signal, int channel) const;

    //-------------------------------------------------------------------------
    void unlockForRead () const {lock_.unlock();}

    //-------------------------------------------------------------------------
    int sampleRate (SignalTypeFlag signal) const;

    //-------------------------------------------------------------------------
    int getSampleLimit (SignalTypeFlag signal) const;

private:
    QMap<quint32, QMap<DeviceID, QList<double> > > aperiodic_data_; // QMap<signal-flag, QMap<device-id, QList-data> >
    QMap<SignalTypeFlag, QHash<int, QVector<double> > > data_;
    QMap<SignalTypeFlag, QHash<int, double> > min_;
    QMap<SignalTypeFlag, QHash<int, double> > max_;
    QMap<SignalTypeFlag, QHash<int, int> > end_index_;
    QMap<SignalTypeFlag, QMap<int, FilteredSignalID> > filter_ids_;
    mutable QHash<SignalTypeFlag, QHash<int, int> > number_new_samples_;
    QHash<SignalTypeFlag, int> sample_limit_;
    QHash<SignalTypeFlag, int> sampling_rate_;
    mutable QReadWriteLock lock_;
};

} // TiAScope

#endif // DATA_BUFFER_H
