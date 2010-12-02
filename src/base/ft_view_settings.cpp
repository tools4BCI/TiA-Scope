#include "ft_view_settings.h"

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
FTViewSettings::FTViewSettings(QObject *parent) :
    QObject(parent)
{
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


} } // namespace tobiss scope
