#ifndef SIGNAL_INFO_UTILS_H
#define SIGNAL_INFO_UTILS_H

#include "data_collector/qt_tia_client/tia_metainfo.h"

#include <QTreeWidget>

//-----------------------------------------------------------------------------
namespace SignalInfoUtils
{
    const int NAME_COLUMN_INDEX = 0;
    const int SAMPLING_RATE_COLUMN_INDEX = 1;
    const int SIGNAL_TYPE_COLUMN_INDEX = 2;
    const int CHANNEL_INDEX_COLUMN_INDEX = 3;

    //-------------------------------------------------------------------------
    void addSignalsToTree (QTreeWidget* tree_widget, TiAQtImplementation::TiAMetaInfo const& signal_info, bool default_checked = true, bool signal_types_checkable = true);
}


#endif // SIGNAL_INFO_UTILS_H
