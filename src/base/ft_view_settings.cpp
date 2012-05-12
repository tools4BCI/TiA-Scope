#include "ft_view_settings.h"

#include "tia/constants.h"

#include <algorithm>

using namespace TiAQtImplementation;

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
//bad hack to undefine signals that is used by Qt
//but at the same time defines a method of SignalInfo
//used in libTiA
#undef signals

FTViewSettings::FTViewSettings (tia::SSConfig const& meta_info, QObject *parent) :
    QObject (parent),
    lower_frequence_bound_ (0),
    upper_frequence_bound_ (0),
    max_sampling_rate_ (0)
{
    tia::Constants tia_constatnts;

    for (tia::SignalInfo::SignalMap::const_iterator signal_iter = meta_info.signal_info.signals().begin ();
         signal_iter != meta_info.signal_info.signals ().end (); ++signal_iter)
    {
        quint32 signal_flag = tia_constatnts.getSignalFlag( signal_iter->second.type());

        sampling_rates_[signal_flag] = signal_iter->second.samplingRate();
        max_sampling_rate_ = std::max<int>(max_sampling_rate_, sampling_rates_[signal_flag]);
    }
    upper_frequence_bound_ = max_sampling_rate_ / 2;
}

//revert hack that signals is undefined
#define signals Q_SIGNALS

//-------------------------------------------------------------------------------------------------
int FTViewSettings::maxSamplingRate () const
{
    return max_sampling_rate_;
}

//-------------------------------------------------------------------------------------------------
int FTViewSettings::samplingRate (SignalTypeFlag signal) const
{
    if (sampling_rates_.contains (signal))
        return sampling_rates_[signal];
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
