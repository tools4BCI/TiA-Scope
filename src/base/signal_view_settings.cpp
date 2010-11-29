#include "signal_view_settings.h"

#include <QSettings>

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
SignalViewSettings::SignalViewSettings(QObject *parent) :
    QObject(parent),
    cyclic_mode_ (false),
    channel_overlapping_ (0)
{
    QSettings settings;
    signal_visualisation_time_ = settings.value ("visualisation/signal_visualisation_time", 2).toDouble();
    basic_y_scaling_ = settings.value ("visualisation/basic_y_scaling", 1).toDouble();
}

//---------------------------------------------------------------------------------------------
bool SignalViewSettings::channelVisible (SignalTypeFlag signal_type, ChannelID channel) const
{
    if (!channel_visibility_.contains(signal_type))
        return false;
    if (!channel_visibility_[signal_type].contains(channel))
        return false;
    return channel_visibility_[signal_type][channel];
}

//---------------------------------------------------------------------------------------------
void SignalViewSettings::setChannelVisibility (SignalTypeFlag signal_type, ChannelID channel, bool visible)
{
    bool changed = true;
    if (channel_visibility_[signal_type].contains(channel))
        if (channel_visibility_[signal_type][channel] == visible)
            changed = false;

    channel_visibility_[signal_type][channel] = visible;

    if (changed)
        Q_EMIT channelVisibilityChanged (signal_type, channel, visible);
}

//---------------------------------------------------------------------------------------------
void SignalViewSettings::setChannelOverlapping (float channel_overlapping)
{
    channel_overlapping_ = channel_overlapping;
    Q_EMIT channelOverlappingChanged (channel_overlapping_);
    Q_EMIT channelOverlappingChanged ();
}




} } // namespace
