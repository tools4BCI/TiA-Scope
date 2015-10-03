#ifndef FILTERS_H
#define FILTERS_H

#include "base/user_types.h"
#include "filters/filter.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QSharedPointer>
#include <QMap>
#include <QMutex>

namespace TiAScope
{


//-----------------------------------------------------------------------------
///
/// Filters
///
/// a singleton class which manages filters
class Filters
{
public:
    //-----------------------------------------------------------------------------
    static Filters& instance ();

    //-----------------------------------------------------------------------------
    ~Filters ();

    //-----------------------------------------------------------------------------
    /// @return a list of available filter names
    QStringList availableFilters () const;

    //-----------------------------------------------------------------------------
    FilterID appendFilter (QString filter_name, QString parameter);    

    //-----------------------------------------------------------------------------
    void removeFilter (FilterID filter_id);

    //-----------------------------------------------------------------------------
    /// delete all filters
    void reset ();

    //-----------------------------------------------------------------------------
    FilteredSignalID registerSignalToBeFiltered (double sampling_rate);

    //-----------------------------------------------------------------------------
    double clock (FilteredSignalID signal, double sample_value);

private:
    //-----------------------------------------------------------------------------
    QSharedPointer<Filter> buildFilter (QString filter_name, double sampling_rate, QString parameter);

    Filters ();

    FilterID next_free_filter_id_;
    FilteredSignalID next_free_filtered_signal_id_;

    QMap<FilterID, QString> applied_filters_;
    QMap<FilterID, QString> applied_filters_parameters_;
    QMap<FilteredSignalID, double> signal_samplingrates_;
    QMap<FilteredSignalID, QMap<FilterID, QSharedPointer<Filter> > > signal_filters_;
    QMutex mutex_;
};

}

#endif // FILTERS_H
