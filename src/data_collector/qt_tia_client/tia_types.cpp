#include "tia_types.h"

#include <QMap>

namespace TiAQtImplementation
{

QMap<SignalTypeFlag, QString> signal_flag_string_map;

//-----------------------------------------------------------------------------
void initSignalFlagStringMap ()
{
    signal_flag_string_map.insert (SIGNAL_TYPE_EEG, SIGNAL_EEG);
    signal_flag_string_map.insert (SIGNAL_TYPE_EMG, SIGNAL_EMG);
    signal_flag_string_map.insert (SIGNAL_TYPE_EOG, SIGNAL_EOG);
    signal_flag_string_map.insert (SIGNAL_TYPE_ECG, SIGNAL_ECG);
    signal_flag_string_map.insert (SIGNAL_TYPE_HR, SIGNAL_HR);
    signal_flag_string_map.insert (SIGNAL_TYPE_BP, SIGNAL_BP);
    signal_flag_string_map.insert (SIGNAL_TYPE_Buttons, SIGNAL_Buttons);
    signal_flag_string_map.insert (SIGNAL_TYPE_Joystick, SIGNAL_Joystick);
    signal_flag_string_map.insert (SIGNAL_TYPE_Sensors, SIGNAL_Sensors);
    signal_flag_string_map.insert (SIGNAL_TYPE_NIRS, SIGNAL_NIRS);
    signal_flag_string_map.insert (SIGNAL_TYPE_FMRI, SIGNAL_FMRI);

    signal_flag_string_map.insert (SIGNAL_TYPE_USER1, SIGNAL_USER_1);
    signal_flag_string_map.insert (SIGNAL_TYPE_USER2, SIGNAL_USER_2);
    signal_flag_string_map.insert (SIGNAL_TYPE_USER3, SIGNAL_USER_3);
    signal_flag_string_map.insert (SIGNAL_TYPE_USER4, SIGNAL_USER_4);
    signal_flag_string_map.insert (SIGNAL_TYPE_UNDEFINED, SIGNAL_UNDEFINED);
    signal_flag_string_map.insert (SIGNAL_TYPE_EVENTS, SIGNAL_EVENT);
}

//-----------------------------------------------------------------------------
SignalTypeFlag toSignalTypeFlag (QString const& str)
{
    if (signal_flag_string_map.size() == 0)
        initSignalFlagStringMap();
    Q_FOREACH (SignalTypeFlag flag, signal_flag_string_map.keys (str))
        return flag;

    return 0;
}

//-----------------------------------------------------------------------------
QString signalTypeFlagToString (SignalTypeFlag flag)
{
    if (signal_flag_string_map.size() == 0)
        initSignalFlagStringMap();
    return signal_flag_string_map[flag];
}

//-----------------------------------------------------------------------------
bool isAperiodic (SignalTypeFlag signal)
{
    return (signal & (SIGNAL_TYPE_Buttons | SIGNAL_TYPE_Joystick));
}


}
