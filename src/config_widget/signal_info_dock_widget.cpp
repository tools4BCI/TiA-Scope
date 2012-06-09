#include "signal_info_dock_widget.h"
#include "ui_signal_info_dock_widget.h"
#include "signal_info_utils.h"

namespace TiAScope
{

//-----------------------------------------------------------------------------
SignalInfoDockWidget::SignalInfoDockWidget (QSharedPointer<SignalViewSettings> settings, QWidget *parent) :
    QDockWidget (parent),
    ui (new Ui::SignalInfoDockWidget),
    initializing_ (false),
    settings_ (settings)
{
    ui->setupUi(this);
    ui->signalTree->setColumnHidden (SignalInfoUtils::SIGNAL_TYPE_COLUMN_INDEX, true);
    ui->signalTree->setColumnHidden (SignalInfoUtils::CHANNEL_INDEX_COLUMN_INDEX, true);
}

//-----------------------------------------------------------------------------
SignalInfoDockWidget::~SignalInfoDockWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------

void SignalInfoDockWidget::setSignalInfo (tia::SSConfig const& signal_info)
{
    initializing_ = true;
    ui->signalTree->clear ();
    SignalInfoUtils::addSignalsToTree (ui->signalTree, signal_info);
    initializing_ = false;
}

//-----------------------------------------------------------------------------
void SignalInfoDockWidget::updateSignalInfo(tia::SSConfig &signal_info)
{
    QTreeWidgetItem *root = ui->signalTree->invisibleRootItem();

    SignalInfoUtils::updateSignalInfo(root,signal_info);
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
    if (column == SignalInfoUtils::NAME_COLUMN_INDEX)
    {

        if(item->checkState(SignalInfoUtils::NAME_COLUMN_INDEX) != Qt::PartiallyChecked)
            for (int index = 0; index < item->childCount(); index++)
                item->child (index)->setCheckState (SignalInfoUtils::NAME_COLUMN_INDEX, item->checkState(SignalInfoUtils::NAME_COLUMN_INDEX));


        if (item->childCount() == 0)
        {
            Q_EMIT channelVisibilityChanged (item->data (SignalInfoUtils::SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole).toUInt(),
                                            item->data (SignalInfoUtils::CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole).toUInt(),
                                            item->checkState(SignalInfoUtils::NAME_COLUMN_INDEX) == Qt::Checked);
        }
    }
//    else if (column == SignalInfoDockWidgetHelper::FT_COLUMN_INDEX)
//    {
//        Q_EMIT signalChannelFTEnabledChanged (item->data (SignalInfoDockWidgetHelper::SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole).toUInt(), item->data (SignalInfoDockWidgetHelper::CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole).toUInt(),
//                                              item->checkState(SignalInfoDockWidgetHelper::FT_COLUMN_INDEX) == Qt::Checked);
//    }
}


} // TiAScope
