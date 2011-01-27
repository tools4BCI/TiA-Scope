#include "ft_view_settings.h"

#include <algorithm>

using namespace TiAQtImplementation;

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
FTViewSettings::FTViewSettings (TiAMetaInfo const& meta_info, QObject *parent) :
    QObject (parent),
    lower_frequence_bound_ (0),
    upper_frequence_bound_ (0),
    max_sampling_rate_ (0)
{
    Q_FOREACH (SignalTypeFlag signal, meta_info.getSignalTypes())
    {
        samping_rates_[signal] = meta_info.getSamplingRate (signal);
        max_sampling_rate_ = std::max<int>(max_sampling_rate_, samping_rates_[signal]);
    }
    upper_frequence_bound_ = max_sampling_rate_ / 2;
}

//-------------------------------------------------------------------------------------------------
int FTViewSettings::maxSamplingRate () const
{
    return max_sampling_rate_;
}

//-------------------------------------------------------------------------------------------------
int FTViewSettings::samplingRate (SignalTypeFlag signal) const
{
    if (samping_rates_.contains (signal))
        return samping_rates_[signal];
    else
        return 0;
}


//-------------------------------------------------------------------------------------------------
void FTViewSettings::setLowerFrequenceBound (int lower_bound)
{
    lower_frequence_bound_ = lower_bound;
    Q_EMIT lowerFrequenceBoundChanged ();
    Q_EMIT lowerFrequenceBoundChanged (lower_frequence_bound_);
}


//-------------------------------------------------------------------------------------------------
void FTViewSettings::setUpperFrequenceBound (int upper_bound)
{
    upper_frequence_bound_ = upper_bound;
    Q_EMIT upperFrequenceBoundChanged ();
    Q_EMIT upperFrequenceBoundChanged (upper_frequence_bound_);
}


} // TiAScope tobiss scope
