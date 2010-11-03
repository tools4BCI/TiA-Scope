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
    Q_PROPERTY (double basicYScaling READ getBasicYScaling WRITE setBasicYScaling)

public:
    //---------------------------------------------------------------------------------------------
    explicit SignalViewSettings(QObject *parent = 0);

    //---------------------------------------------------------------------------------------------
    bool getCyclicMode () const {return cyclic_mode_;}

    //---------------------------------------------------------------------------------------------
    double getSignalVisualisationTime () const {return signal_visualisation_time_;}

    //---------------------------------------------------------------------------------------------
    double getBasicYScaling () const {return basic_y_scaling_;}

public Q_SLOTS:
    //---------------------------------------------------------------------------------------------
    void setSignalVisualisationTime (double seconds) {signal_visualisation_time_ = seconds; Q_EMIT signalVisualisationTimeChanged (signal_visualisation_time_);}

    //---------------------------------------------------------------------------------------------
    void setBasicYScaling (double basicYScaling) {basic_y_scaling_ = basicYScaling;}

    //---------------------------------------------------------------------------------------------
    void setCyclicMode (bool cyclic_mode) {cyclic_mode_ = cyclic_mode;}

Q_SIGNALS:
    //---------------------------------------------------------------------------------------------
    void signalVisualisationTimeChanged (double seconds);

private:
    double signal_visualisation_time_;
    double basic_y_scaling_;
    bool cyclic_mode_;
};

} } // namespace

#endif // SIGNAL_VIEW_SETTINGS_H
