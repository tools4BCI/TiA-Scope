#ifndef TIA_METAINFO_H
#define TIA_METAINFO_H

#include "tia_types.h"

#include <QMap>
#include <QSet>
#include <QString>

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
class TiAMetaInfo
{
public:
    TiAMetaInfo () {}

//    TiAMetaInfo (QSet<SignalTypeFlag> signal_types)
//        : signal_types_ (signal_types)
//    {}

//    TiAMetaInfo (QSet<SignalTypeFlag> signal_types, QMap<QString, QString> subject_info)
//        : signal_types_ (signal_types), subject_info_ (subject_info)
//    {}

//    TiAMetaInfo (TiAMetaInfo const& src)
//        : signal_types_ (src.signal_types_),
//          sampling_rates_ (src.sampling_rates_),
//          channel_labels_ (src.channel_labels_),
//          subject_info_ (src.subject_info_)
//    {}

    void addSignal (SignalTypeFlag signal) {signal_types_.insert (signal);}
    void setSamplingRate (SignalTypeFlag signal, double sampling_rate) {sampling_rates_[signal] = sampling_rate;}
    void setChannelLabel (SignalTypeFlag signal, ChannelIndex channel, QString label) {channel_labels_[signal][channel] = label;}

    QSet<SignalTypeFlag> getSignalTypes () const {return signal_types_;}
    QMap<QString, QString> getSubjectInfo () const {return subject_info_;}
    double getSamplingRate (SignalTypeFlag signal) const {return sampling_rates_[signal];}
    QString getChannelLabel (SignalTypeFlag signal, ChannelIndex channel) const {return channel_labels_[signal][channel];}

private:
    QSet<SignalTypeFlag> signal_types_;
    QMap<SignalTypeFlag, double> sampling_rates_;
    QMap<SignalTypeFlag, QMap<ChannelIndex, QString> > channel_labels_;
    QMap<QString, QString> subject_info_;
};

}

#endif // TIA_METAINFO_H
