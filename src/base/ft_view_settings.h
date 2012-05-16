#ifndef FT_VIEW_SETTINGS_H
#define FT_VIEW_SETTINGS_H

#include "tia/defines.h"

#include "tia/ssconfig.h"

#include <QObject>
#include <QMap>


namespace TiAScope {

class FTViewSettings : public QObject
{
    Q_OBJECT

public:    
    explicit FTViewSettings (tia::SSConfig const& meta_info, QObject *parent = 0);

    int maxSamplingRate () const;

    int samplingRate (SignalTypeFlag signal) const;

    int lowerFrequenceBound () const {return lower_frequence_bound_;}

    int upperFrequenceBound () const {return upper_frequence_bound_;}

Q_SIGNALS:
    void lowerFrequenceBoundChanged (int lower_bound);
    void lowerFrequenceBoundChanged ();
    void upperFrequenceBoundChanged (int upper_bound);
    void upperFrequenceBoundChanged ();

public Q_SLOTS:
    void setLowerFrequenceBound (int lower_bound);
    void setUpperFrequenceBound (int upper_bound);

private:
    int lower_frequence_bound_;
    int upper_frequence_bound_;
    QMap<SignalTypeFlag, int> sampling_rates_;
    int max_sampling_rate_;
};

} // TiAScope tobiss scope

#endif // FT_VIEW_SETTINGS_H
