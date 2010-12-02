#ifndef FT_VIEW_SETTINGS_H
#define FT_VIEW_SETTINGS_H

#include "user_types.h"

#include <QObject>

namespace tobiss { namespace scope {

class FTViewSettings : public QObject
{
    Q_OBJECT

public:
    explicit FTViewSettings (QObject *parent = 0);

    //int samplingRate (SignalTypeFlag signal, ChannelID channel) const;

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

};

} } // namespace tobiss scope

#endif // FT_VIEW_SETTINGS_H
