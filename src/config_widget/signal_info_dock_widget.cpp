#include "signal_info_dock_widget.h"
#include "ui_signal_info_dock_widget.h"

namespace tobiss { namespace scope {

namespace SignalInfoDockWidgetHelper
{
    const int NAME_COLUMN_INDEX = 0;
    const int SAMPLING_RATE_COLUMN_INDEX = 1;
    const int FT_COLUMN_INDEX = 2;
    const int SIGNAL_TYPE_COLUMN_INDEX = 3;
    const int CHANNEL_INDEX_COLUMN_INDEX = 4;

    //-------------------------------------------------------------------------
    /// adds the given signals as a row to the given tree
    void addSignalsToTree (QTreeWidget* tree_widget, SignalInfo::SignalMap const& signal_map);
}

//-----------------------------------------------------------------------------
SignalInfoDockWidget::SignalInfoDockWidget (QWidget *parent) :
    QDockWidget (parent),
    ui (new Ui::SignalInfoDockWidget),
    initializing_ (false)
{
    ui->setupUi(this);
    ui->signalTree->setColumnHidden (SignalInfoDockWidgetHelper::SIGNAL_TYPE_COLUMN_INDEX, true);
    ui->signalTree->setColumnHidden (SignalInfoDockWidgetHelper::CHANNEL_INDEX_COLUMN_INDEX, true);
    ui->signalTree->setColumnWidth (SignalInfoDockWidgetHelper::FT_COLUMN_INDEX, 20);
}

//-----------------------------------------------------------------------------
SignalInfoDockWidget::~SignalInfoDockWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------
void SignalInfoDockWidget::setSignalInfo (SignalInfo const& signal_info)
{
    initializing_ = true;
    ui->signalTree->clear ();
    SignalInfoDockWidgetHelper::addSignalsToTree (ui->signalTree, signal_info.signals());
    initializing_ = false;
}

//-----------------------------------------------------------------------------
void SignalInfoDockWidget::clear ()
{
    ui->signalTree->clear();
}


//-----------------------------------------------------------------------------
void SignalInfoDockWidget::on_signalTree_itemChanged (QTreeWidgetItem* item, int column)
{
    if (initializing_)
        return;
    if (column == SignalInfoDockWidgetHelper::NAME_COLUMN_INDEX)
    {
        for (int index = 0; index < item->childCount(); index++)
            item->child (index)->setCheckState (SignalInfoDockWidgetHelper::NAME_COLUMN_INDEX, item->checkState(SignalInfoDockWidgetHelper::NAME_COLUMN_INDEX));
        if (item->childCount() == 0)
        {
            Q_EMIT channelVisibilityChanged (item->data (SignalInfoDockWidgetHelper::SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole).toUInt(),
                                            item->data (SignalInfoDockWidgetHelper::CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole).toUInt(),
                                            item->checkState(SignalInfoDockWidgetHelper::NAME_COLUMN_INDEX) == Qt::Checked);
        }
    }
    else if (column == SignalInfoDockWidgetHelper::FT_COLUMN_INDEX)
    {
        Q_EMIT signalChannelFTEnabledChanged (item->data (SignalInfoDockWidgetHelper::SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole).toUInt(), item->data (SignalInfoDockWidgetHelper::CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole).toUInt(),
                                              item->checkState(SignalInfoDockWidgetHelper::FT_COLUMN_INDEX) == Qt::Checked);
    }
}

//-----------------------------------------------------------------------------
namespace SignalInfoDockWidgetHelper
{
    //-------------------------------------------------------------------------
    void addSignalsToTree (QTreeWidget* tree_widget, SignalInfo::SignalMap const& signal_map)
    {
        for (SignalInfo::SignalMap::const_iterator signal_iter = signal_map.begin();
             signal_iter != signal_map.end();
             ++signal_iter)
        {
            QTreeWidgetItem* signal_item = new QTreeWidgetItem (tree_widget);
            signal_item->setData (SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole, TypeConverter::stdStringToSignalTypeFlag (signal_iter->first.c_str()));
            signal_item->setText (NAME_COLUMN_INDEX, QString (signal_iter->first.c_str()));
            signal_item->setFlags (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            signal_item->setCheckState (NAME_COLUMN_INDEX, Qt::Checked);
            signal_item->setText (SAMPLING_RATE_COLUMN_INDEX, QString::number (signal_iter->second.samplingRate ()).append( " Hz"));
            unsigned index = 0;
            Q_FOREACH (Channel channel, signal_iter->second.channels ())
            {
                QTreeWidgetItem* channel_item = new QTreeWidgetItem (signal_item);
                channel_item->setFlags (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                channel_item->setCheckState (NAME_COLUMN_INDEX, Qt::Checked);
                channel_item->setData (SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole, TypeConverter::stdStringToSignalTypeFlag (signal_iter->first.c_str()));
                channel_item->setData (CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole, index);
                channel_item->setText (NAME_COLUMN_INDEX, channel.id().c_str());
                channel_item->setCheckState (FT_COLUMN_INDEX, Qt::Unchecked);
                index++;
            }
        }
    }
}

} } // namespace
