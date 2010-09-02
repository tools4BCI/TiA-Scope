#ifndef SIGNAL_VIEW_SETTINGS_H
#define SIGNAL_VIEW_SETTINGS_H

#include <QObject>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
/// SignalViewSettings
///
/// a kind of mediator
class SignalViewSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY (double signalVisualisationTime READ getSignalVisualisationTime WRITE setSignalVisualisationTime)

public:
    //---------------------------------------------------------------------------------------------
    explicit SignalViewSettings(QObject *parent = 0);

    //---------------------------------------------------------------------------------------------
    bool getCyclicMode () const {return cyclic_mode_;}

    //---------------------------------------------------------------------------------------------
    double getSignalVisualisationTime () const {return signal_visualisation_time_;}

public Q_SLOTS:
    //---------------------------------------------------------------------------------------------
    void setSignalVisualisationTime (double seconds) {signal_visualisation_time_ = seconds; Q_EMIT signalVisualisationTimeChanged (signal_visualisation_time_);}

    //---------------------------------------------------------------------------------------------
    void setCyclicMode (bool cyclic_mode) {cyclic_mode_ = cyclic_mode;}

Q_SIGNALS:
    //---------------------------------------------------------------------------------------------
    void signalVisualisationTimeChanged (double seconds);

private:
    double signal_visualisation_time_;
    bool cyclic_mode_;
};

} } // namespace

#endif // SIGNAL_VIEW_SETTINGS_H
