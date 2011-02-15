#include "signal_info_utils.h"

namespace SignalInfoUtils
{

//-------------------------------------------------------------------------
void addSignalsToTree (QTreeWidget* tree_widget, TiAQtImplementation::TiAMetaInfo const& signal_info, bool default_checked, bool signal_types_checkable)
{
    Q_FOREACH (TiAQtImplementation::SignalTypeFlag signal, signal_info.getSignalTypes())
    {
        QTreeWidgetItem* signal_item = new QTreeWidgetItem (tree_widget);
        signal_item->setData (SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole, signal);
        signal_item->setText (NAME_COLUMN_INDEX, TiAQtImplementation::signalTypeFlagToString (signal));
        signal_item->setFlags (Qt::ItemIsEnabled);
        if (signal_types_checkable)
        {
            signal_item->setFlags (signal_item->flags() | Qt::ItemIsUserCheckable);
            if (default_checked)
                signal_item->setCheckState (NAME_COLUMN_INDEX, Qt::Checked);
        }
        signal_item->setText (SAMPLING_RATE_COLUMN_INDEX, QString::number (signal_info.getSamplingRate(signal)).append( " Hz"));
        unsigned index = 0;
        for (TiAQtImplementation::ChannelIndex channel = 0; channel < signal_info.getNumChannels (signal); ++channel)
        {
            QTreeWidgetItem* channel_item = new QTreeWidgetItem (signal_item);
            channel_item->setFlags (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            if (default_checked)
                channel_item->setCheckState (NAME_COLUMN_INDEX, Qt::Checked);
            else
                channel_item->setCheckState (NAME_COLUMN_INDEX, Qt::Unchecked);
            channel_item->setData (SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole, signal);
            channel_item->setData (CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole, index);
            channel_item->setText (NAME_COLUMN_INDEX, signal_info.getChannelLabel (signal, channel));
            index++;
        }
    }
}

}
