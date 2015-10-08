#include "filters.h"
#include "filters/butterworth_filter.h"
#include "filters/butterworth_highpass.h"

#include <QMutexLocker>

namespace TiAScope
{

//-----------------------------------------------------------------------------
Filters& Filters::instance ()
{
    static Filters obj_instance;
    return obj_instance;
}

//-----------------------------------------------------------------------------
Filters::~Filters ()
{

}

//-----------------------------------------------------------------------------
Filters::Filters () :
        next_free_filter_id_ (1),
        next_free_filtered_signal_id_ (1)
{
}

//-----------------------------------------------------------------------------
QStringList Filters::availableFilters () const
{
    return QStringList () << "Butterworth" << "ButterworthHighpass";
}

//-----------------------------------------------------------------------------
FilterID Filters::appendFilter (QString filter_name, QString parameter)
{
    QMutexLocker locker (&mutex_);

    FilterID new_filter_id = next_free_filter_id_;
    next_free_filter_id_++;

    applied_filters_[new_filter_id] = filter_name;
    applied_filters_parameters_[new_filter_id] = parameter;
    Q_FOREACH (FilteredSignalID signal_id, signal_filters_.keys())
    {
        signal_filters_[signal_id].insert(new_filter_id, buildFilter (filter_name, signal_samplingrates_[signal_id], parameter));
    }
    return new_filter_id;
}

//-----------------------------------------------------------------------------
void Filters::removeFilter(FilterID filter_id)
{
    QMutexLocker locker (&mutex_);

    applied_filters_.remove(filter_id);
    applied_filters_parameters_.remove(filter_id);

    Q_FOREACH (FilteredSignalID signal_id, signal_filters_.keys())
    {
        signal_filters_[signal_id].remove(filter_id);
    }
}

//-----------------------------------------------------------------------------
void Filters::reset ()
{
    QMutexLocker locker (&mutex_);

    applied_filters_.clear ();

    Q_FOREACH (FilteredSignalID signal_id, signal_filters_.keys())
    {
        signal_filters_[signal_id].clear ();
    }
}

//-----------------------------------------------------------------------------
FilteredSignalID Filters::registerSignalToBeFiltered (double sampling_rate)
{
    QMutexLocker locker (&mutex_);

    FilteredSignalID signal_id = next_free_filtered_signal_id_;
    next_free_filtered_signal_id_++;

    signal_samplingrates_.insert (signal_id, sampling_rate);

    Q_FOREACH (FilterID filter_id, applied_filters_.keys ())
    {
        signal_filters_[signal_id].insert(filter_id, buildFilter (applied_filters_[filter_id], sampling_rate, applied_filters_parameters_[filter_id]));
    }

    return signal_id;
}

//-----------------------------------------------------------------------------
double Filters::clock (FilteredSignalID signal, double sample_value)
{
    QMutexLocker locker (&mutex_);
    double filtered_value = sample_value;
    Q_FOREACH (QSharedPointer<Filter> filter, signal_filters_[signal])
    {
        filtered_value = filter->clock (filtered_value);
    }
    return filtered_value;
}

//-----------------------------------------------------------------------------
QSharedPointer<Filter> Filters::buildFilter (QString filter_name, double sampling_rate, QString parameter)
{
    if (filter_name == QString ("Butterworth"))
        return QSharedPointer<Filter> (new ButterworthFilter (sampling_rate, parameter.toDouble()));
    if (filter_name == QString ("ButterworthHighpass"))
        return QSharedPointer<Filter> (new ButterworthHighpass (parameter.toInt()));
    return QSharedPointer<Filter> ();
}


}
