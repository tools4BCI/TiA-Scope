#ifndef TIA_METAINFO_H
#define TIA_METAINFO_H

#include "tia_types.h"

#include <QMap>
#include <QSet>
#include <QString>

namespace TiAQtImplementation
{

typedef QMap<QString, QString> SubjectInfoMap;

//-----------------------------------------------------------------------------
class TiAMetaInfo
{
public:
    void addSignal (SignalTypeFlag signal, ChannelIndex num_channels) {signal_types_.insert (signal); num_channels_.insert(signal, num_channels);}
    void setSamplingRate (SignalTypeFlag signal, double sampling_rate) {sampling_rates_[signal] = sampling_rate;}
    void setChannelLabel (SignalTypeFlag signal, ChannelIndex channel, QString label) {channel_labels_[signal][channel] = label;}

    QSet<SignalTypeFlag> getSignalTypes () const {return signal_types_;}
    ChannelIndex getNumChannels (SignalTypeFlag signal) const {return num_channels_[signal];}
    QMap<QString, QString> getSubjectInfo () const {return subject_info_;}
    double getSamplingRate (SignalTypeFlag signal) const {return sampling_rates_[signal];}
    QString getChannelLabel (SignalTypeFlag signal, ChannelIndex channel) const {return channel_labels_[signal][channel];}

private:
    QMap<SignalTypeFlag, ChannelIndex> num_channels_;
    QSet<SignalTypeFlag> signal_types_;
    QMap<SignalTypeFlag, double> sampling_rates_;
    QMap<SignalTypeFlag, QMap<ChannelIndex, QString> > channel_labels_;
    SubjectInfoMap subject_info_;
};

}

#endif // TIA_METAINFO_H
