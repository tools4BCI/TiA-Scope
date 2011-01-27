#ifndef TIA_TYPES_H
#define TIA_TYPES_H

#include <QtGlobal>
#include <QString>

namespace TiAQtImplementation
{

typedef quint32 SignalTypeFlag;
typedef quint32 ChannelIndex;

SignalTypeFlag const SIGNAL_TYPE_EEG = 0x1;
SignalTypeFlag const SIGNAL_TYPE_EMG = 0x1 << 1;
SignalTypeFlag const SIGNAL_TYPE_EOG = 0x1 << 2;
SignalTypeFlag const SIGNAL_TYPE_ECG = 0x1 << 3;
SignalTypeFlag const SIGNAL_TYPE_HR = 0x1 << 4;
SignalTypeFlag const SIGNAL_TYPE_BP = 0x1 << 5;
SignalTypeFlag const SIGNAL_TYPE_Buttons = 0x1 << 6;
SignalTypeFlag const SIGNAL_TYPE_Joystick = 0x1 << 7;
SignalTypeFlag const SIGNAL_TYPE_Sensors = 0x1 << 8;
SignalTypeFlag const SIGNAL_TYPE_NIRS = 0x1 << 9;
SignalTypeFlag const SIGNAL_TYPE_FMRI = 0x1 << 10;
SignalTypeFlag const SIGNAL_TYPE_UNDEFINED1 = 0x1 << 11;
SignalTypeFlag const SIGNAL_TYPE_UNDEFINED2 = 0x1 << 12;
SignalTypeFlag const SIGNAL_TYPE_UNDEFINED3 = 0x1 << 13;
SignalTypeFlag const SIGNAL_TYPE_UNDEFINED4 = 0x1 << 14;
SignalTypeFlag const SIGNAL_TYPE_UNDEFINED5 = 0x1 << 15;
SignalTypeFlag const SIGNAL_TYPE_USER1 = 0x1 << 16;
SignalTypeFlag const SIGNAL_TYPE_USER2 = 0x1 << 17;
SignalTypeFlag const SIGNAL_TYPE_USER3 = 0x1 << 18;
SignalTypeFlag const SIGNAL_TYPE_USER4 = 0x1 << 19;
SignalTypeFlag const SIGNAL_TYPE_UNDEFINED = 0x1 << 20;
SignalTypeFlag const SIGNAL_TYPE_EVENTS = 0x1 << 21;

QString const SIGNAL_EEG = "eeg";
QString const SIGNAL_EMG = "emg";
QString const SIGNAL_EOG = "eog";
QString const SIGNAL_ECG = "ecg";
QString const SIGNAL_HR = "hr";
QString const SIGNAL_BP = "bp";
QString const SIGNAL_Buttons = "button";
QString const SIGNAL_Joystick = "joystick";
QString const SIGNAL_Sensors = "sensor";
QString const SIGNAL_NIRS = "nirs";
QString const SIGNAL_FMRI = "fmri";

//-----------------------------------------------------------------------------
SignalTypeFlag toSignalTypeFlag (QString const& str);

//-----------------------------------------------------------------------------
QString signalTypeFlagToString (SignalTypeFlag flag);

//-----------------------------------------------------------------------------
bool isAperiodic (SignalTypeFlag signal);

}

#endif // TIA_TYPES_H
