#ifndef SIGNAL_INFO_UTILS_H
#define SIGNAL_INFO_UTILS_H

#include "tia/ssconfig.h"

#include <QTreeWidget>

//-----------------------------------------------------------------------------
namespace SignalInfoUtils
{
    const int NAME_COLUMN_INDEX = 0;
    const int SAMPLING_RATE_COLUMN_INDEX = 1;
    const int SIGNAL_TYPE_COLUMN_INDEX = 2;
    const int CHANNEL_INDEX_COLUMN_INDEX = 3;

    //-------------------------------------------------------------------------    
    void addSignalsToTree (QTreeWidget* tree_widget, tia::SSConfig const& meta_info, bool default_checked = true, bool signal_types_checkable = true);


    //-------------------------------------------------------------------------
    void updateSignalInfo(QTreeWidgetItem* tree_widget, tia::SSConfig &meta_info);
}


#endif // SIGNAL_INFO_UTILS_H
