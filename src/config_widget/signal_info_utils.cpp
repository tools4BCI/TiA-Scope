#include "signal_info_utils.h"

#include "tia/constants.h"

#include <stdexcept>
#include <iostream>

namespace SignalInfoUtils
{

//-----------------------------------------------------------------------------
//bad hack to undefine signals that is used by Qt
//but at the same time defines a method of SignalInfo
//used in libTiA
#undef signals

void addSignalsToTree (QTreeWidget* tree_widget, tia::SSConfig const& meta_info, bool default_checked, bool signal_types_checkable)
{
    tia::Constants tia_constants;

    for (tia::SignalInfo::SignalMap::const_iterator signal_iter = meta_info.signal_info.signals().begin ();
         signal_iter != meta_info.signal_info.signals ().end (); ++signal_iter)
    {
        quint32 signal_flag = tia_constants.getSignalFlag( signal_iter->second.type());

        QTreeWidgetItem* signal_item = new QTreeWidgetItem (tree_widget);

        signal_item->setData (SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole, signal_flag);
        signal_item->setText (NAME_COLUMN_INDEX, signal_iter->second.type().c_str());
        signal_item->setFlags (Qt::ItemIsEnabled);

        if (signal_types_checkable)
        {
            signal_item->setFlags (signal_item->flags() | Qt::ItemIsUserCheckable);
            signal_item->setFlags (signal_item->flags() | Qt::ItemIsTristate);
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
            channel_item->setData (CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole, signal_iter->second.channels()[channel_index].number());
            channel_item->setText (NAME_COLUMN_INDEX, signal_iter->second.channels ()[channel_index].id().c_str());
            index++;
        }
    }
}

//-----------------------------------------------------------------------------
void updateSignalInfo(QTreeWidgetItem* root, tia::SSConfig &meta_info)
{
    tia::Constants tia_constants;

    for (int sig_tree_index = 0; sig_tree_index < root->childCount(); ++sig_tree_index)
    {
        QTreeWidgetItem *signal_item = root->child(sig_tree_index);

        quint32 signal_flag = (quint32)signal_item->data(SIGNAL_TYPE_COLUMN_INDEX,Qt::UserRole).toInt();

        if(signal_item->checkState(NAME_COLUMN_INDEX) == Qt::Checked)
            ; //nothing to do, all channels of the signal are accepted
        else
        {
            tia::SignalInfo::SignalMap::iterator iter = meta_info.signal_info.signals().find(tia_constants.getSignalName(signal_flag));

            if(iter == meta_info.signal_info.signals().end())
                throw std::runtime_error("ReaderThread::run(): Datapacket contains signal that is not in the meta info!");
            else if(signal_item->checkState(NAME_COLUMN_INDEX) == Qt::Unchecked)
            {
                //delete whole signal form meta_info
                meta_info.signal_info.signals().erase(iter);
            }
            else
            {
                //check state is intermediate => not all channels are deleted
                tia::Signal &signal = iter->second;

                for (int chan_tree_idx = 0; chan_tree_idx < signal_item->childCount(); ++chan_tree_idx)
                {
                    QTreeWidgetItem *chan_item = signal_item->child(chan_tree_idx);
                    if(chan_item->checkState(NAME_COLUMN_INDEX) == Qt::Unchecked)
                    {
                        //add channel to new vector
                        std::vector<tia::Channel> &channels = signal.channels();

                        std::vector<tia::Channel>::iterator chan_iter = channels.begin();

                        for(; chan_iter != channels.end();)
                        {
                            if(chan_iter->number() == chan_item->data(CHANNEL_INDEX_COLUMN_INDEX,Qt::UserRole).toInt())
                                chan_iter = channels.erase(chan_iter);
                            else
                                ++chan_iter;
                        }
                    }

                }

            }

        }
    }

}

//revert hack that signals is undefined
#define signals Q_SIGNALS

}
