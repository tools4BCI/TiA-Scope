#include "ft_view_settings.h"

#include "tia/ssconfig.h"

#include <algorithm>

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
FTViewSettings::FTViewSettings (tobiss::SSConfig const& ss_config, QObject *parent) :
    QObject (parent),
    lower_frequence_bound_ (0),
    upper_frequence_bound_ (0),
    max_sampling_rate_ (0)
{
    for (tobiss::SignalInfo::SignalMap::const_iterator signal_iter = ss_config.signal_info.signals().begin ();
         signal_iter != ss_config.signal_info.signals().end (); ++signal_iter)
    {
        samping_rates_[TypeConverter::stdStringToSignalTypeFlag (signal_iter->first)] = signal_iter->second.samplingRate ();
        max_sampling_rate_ = std::max<int>(max_sampling_rate_, signal_iter->second.samplingRate ());
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
