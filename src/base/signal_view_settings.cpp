#include "signal_view_settings.h"

#include <QSettings>

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
SignalViewSettings::SignalViewSettings(QObject *parent) :
    QObject(parent),
    cyclic_mode_ (false)
{
    QSettings settings;
    signal_visualisation_time_ = settings.value ("visualisation/signal_visualisation_time", 2).toDouble();
}

} } // namespace
