#include "signal_info_utils.h"

#include "tia/constants.h"

namespace SignalInfoUtils
{

//-----------------------------------------------------------------------------
//bad hack to undefine signals that is used by Qt
//but at the same time defines a method of SignalInfo
//used in libTiA
#undef signals

void addSignalsToTree (QTreeWidget* tree_widget, tia::SSConfig const& meta_info, bool default_checked, bool signal_types_checkable)
{
    tia::Constants tia_constatnts;

    for (tia::SignalInfo::SignalMap::const_iterator signal_iter = meta_info.signal_info.signals().begin ();
         signal_iter != meta_info.signal_info.signals ().end (); ++signal_iter)
    {
        quint32 signal_flag = tia_constatnts.getSignalFlag( signal_iter->second.type());

        QTreeWidgetItem* signal_item = new QTreeWidgetItem (tree_widget);

        signal_item->setData (SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole, signal_flag);
        signal_item->setText (NAME_COLUMN_INDEX, signal_iter->second.type().c_str());
        signal_item->setFlags (Qt::ItemIsEnabled);

        if (signal_types_checkable)
        {
            signal_item->setFlags (signal_item->flags() | Qt::ItemIsUserCheckable);
            if (default_checked)
                signal_item->setCheckState (NAME_COLUMN_INDEX, Qt::Checked);
        }
        signal_item->setText (SAMPLING_RATE_COLUMN_INDEX, QString::number (signal_iter->second.samplingRate()).append( " Hz"));

        unsigned index = 0;

        for (unsigned channel_index = 0; channel_index < signal_iter->second.channels().size(); channel_index++)
        {
            QTreeWidgetItem* channel_item = new QTreeWidgetItem (signal_item);
            channel_item->setFlags (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            if (default_checked)
                channel_item->setCheckState (NAME_COLUMN_INDEX, Qt::Checked);
            else
                channel_item->setCheckState (NAME_COLUMN_INDEX, Qt::Unchecked);
            channel_item->setData (SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole, signal_flag);
            channel_item->setData (CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole, index);
            channel_item->setText (NAME_COLUMN_INDEX, signal_iter->second.channels ()[channel_index].id().c_str());
            index++;
        }
    }
}

//revert hack that signals is undefined
#define signals Q_SIGNALS

}
