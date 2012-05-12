#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "user_types.h"

#include "data_collector/qt_tia_client/tia_types.h"
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
    void appendData (TiAQtImplementation::SignalTypeFlag signal, int channel, QList<double> const& data);

    //-------------------------------------------------------------------------
    void setAperiodicValues (TiAQtImplementation::SignalTypeFlag signal_flag, DeviceID device_id, QList<double> const& data);

    //-------------------------------------------------------------------------
    QList<DeviceID> getAperiodicDeviceIDs (TiAQtImplementation::SignalTypeFlag signal_flag) const;

    //-------------------------------------------------------------------------
    QList<double> getAperiodicValues (TiAQtImplementation::SignalTypeFlag signal_flag, DeviceID device_id) const;

    //-------------------------------------------------------------------------
    void lockForRead () const {lock_.lockForRead();}

    //-------------------------------------------------------------------------
    double getData (TiAQtImplementation::SignalTypeFlag signal, int channel, int sample_index) const;

    //-------------------------------------------------------------------------
    double getMin (TiAQtImplementation::SignalTypeFlag signal, int channel) const;

    //-------------------------------------------------------------------------
    double getMax (TiAQtImplementation::SignalTypeFlag signal, int channel) const;

    //-------------------------------------------------------------------------
    /// fill the data_array with the last samples of the given channel
    void getData (TiAQtImplementation::SignalTypeFlag signal, int channel, QVarLengthArray<double>& data_array) const;

    //-------------------------------------------------------------------------
    int numberNewSamples (TiAQtImplementation::SignalTypeFlag signal, int channel) const;

    //-------------------------------------------------------------------------
    void unlockForRead () const {lock_.unlock();}

    //-------------------------------------------------------------------------
    int sampleRate (TiAQtImplementation::SignalTypeFlag signal) const;

    //-------------------------------------------------------------------------
    int getSampleLimit (TiAQtImplementation::SignalTypeFlag signal) const;

private:
    QMap<quint32, QMap<DeviceID, QList<double> > > aperiodic_data_; // QMap<signal-flag, QMap<device-id, QList-data> >
    QMap<TiAQtImplementation::SignalTypeFlag, QHash<int, QVector<double> > > data_;
    QMap<TiAQtImplementation::SignalTypeFlag, QHash<int, double> > min_;
    QMap<TiAQtImplementation::SignalTypeFlag, QHash<int, double> > max_;
    QMap<TiAQtImplementation::SignalTypeFlag, QHash<int, int> > end_index_;
    QMap<TiAQtImplementation::SignalTypeFlag, QMap<int, FilteredSignalID> > filter_ids_;
    mutable QHash<TiAQtImplementation::SignalTypeFlag, QHash<int, int> > number_new_samples_;
    QHash<TiAQtImplementation::SignalTypeFlag, int> sample_limit_;
    QHash<TiAQtImplementation::SignalTypeFlag, int> sampling_rate_;
    mutable QReadWriteLock lock_;
};

} // TiAScope

#endif // DATA_BUFFER_H
