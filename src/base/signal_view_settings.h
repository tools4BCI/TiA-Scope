#ifndef SIGNAL_VIEW_SETTINGS_H
#define SIGNAL_VIEW_SETTINGS_H

#include "user_types.h"

#include <QMap>
#include <QObject>

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
/// SignalViewSettings
///
/// a kind of mediator
class SignalViewSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY (double signalVisualisationTime READ getSignalVisualisationTime WRITE setSignalVisualisationTime)
    Q_PROPERTY (double basicYScaling READ getBasicYScaling WRITE setBasicYScaling)
    Q_PROPERTY (float channelOverlapping READ getChannelOverlapping WRITE setChannelOverlapping)

public:
    //---------------------------------------------------------------------------------------------
    explicit SignalViewSettings(QObject *parent = 0);

    //---------------------------------------------------------------------------------------------
    bool autoScalingEnabled () const {return auto_scaling_;}

    //---------------------------------------------------------------------------------------------
    bool getCyclicMode () const {return cyclic_mode_;}

    //---------------------------------------------------------------------------------------------
    double getSignalVisualisationTime () const {return signal_visualisation_time_;}

    //---------------------------------------------------------------------------------------------
    double getBasicYScaling () const {return basic_y_scaling_;}

    //---------------------------------------------------------------------------------------------
    bool channelVisible (SignalTypeFlag signal_type, ChannelID channel) const;

    //---------------------------------------------------------------------------------------------
    float getChannelOverlapping () const {return channel_overlapping_;}
public Q_SLOTS:
    //---------------------------------------------------------------------------------------------
    void setAutoScalingEnabled (bool enabled) {auto_scaling_ = enabled;}

    //---------------------------------------------------------------------------------------------
    void setSignalVisualisationTime (double seconds) {signal_visualisation_time_ = seconds; Q_EMIT signalVisualisationTimeChanged (signal_visualisation_time_);}

    //---------------------------------------------------------------------------------------------
    void setBasicYScaling (double basicYScaling) {basic_y_scaling_ = basicYScaling;}

    //---------------------------------------------------------------------------------------------
    void setCyclicMode (bool cyclic_mode) {cyclic_mode_ = cyclic_mode;}

    //---------------------------------------------------------------------------------------------
    void setChannelVisibility (SignalTypeFlag signal_type, ChannelID channel, bool visible);

    //---------------------------------------------------------------------------------------------
    void setChannelOverlapping (float channel_overlapping);
Q_SIGNALS:
    //---------------------------------------------------------------------------------------------
    void signalVisualisationTimeChanged (double seconds);

    //---------------------------------------------------------------------------------------------
    void channelVisibilityChanged (SignalTypeFlag signal_type, ChannelID channel, bool visible);

    //---------------------------------------------------------------------------------------------
    void channelOverlappingChanged (float channel_overlapping);

    //---------------------------------------------------------------------------------------------
    void channelOverlappingChanged ();
private:
    bool auto_scaling_;
    double signal_visualisation_time_;
    double basic_y_scaling_;
    bool cyclic_mode_;
    float channel_overlapping_;
    QMap<SignalTypeFlag, QMap<ChannelID, bool> > channel_visibility_;
};

} // TiAScope

#endif // SIGNAL_VIEW_SETTINGS_H
