#ifndef FT_VIEW_SETTINGS_H
#define FT_VIEW_SETTINGS_H

#include "data_collector/qt_tia_client/tia_metainfo.h"

#include <QObject>
#include <QMap>


namespace TiAScope {

class FTViewSettings : public QObject
{
    Q_OBJECT

public:
    explicit FTViewSettings (TiAQtImplementation::TiAMetaInfo const& meta_info, QObject *parent = 0);

    int maxSamplingRate () const;

    int samplingRate (TiAQtImplementation::SignalTypeFlag signal) const;

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
    QMap<TiAQtImplementation::SignalTypeFlag, int> samping_rates_;
    int max_sampling_rate_;
};

} // TiAScope tobiss scope

#endif // FT_VIEW_SETTINGS_H
