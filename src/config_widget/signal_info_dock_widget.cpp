#include "signal_info_dock_widget.h"
#include "ui_signal_info_dock_widget.h"
#include "signal_info_utils.h"

#include "tia/constants.h"

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

void SignalInfoDockWidget::setSignalInfo (tia::SSConfig const& signal_info, bool use_channel_numbers)
{
    initializing_ = true;
    ui->signalTree->clear ();        

    SignalInfoUtils::addSignalsToTree (ui->signalTree, signal_info, use_channel_numbers);


    tia::SignalInfo::SignalMap::const_iterator signal_iter =
            signal_info.signal_info.signals().begin ();
    if(signal_iter != signal_info.signal_info.signals ().end ())
    {
        tia::Constants tia_constants;
        quint16 first_signal = tia_constants.getSignalFlag(signal_iter->first);

        Q_EMIT selectedSignalChanged(first_signal);
    }

    initializing_ = false;
}

//-----------------------------------------------------------------------------
void SignalInfoDockWidget::updateSignalInfo(tia::CustomSignalInfoPtr signal_info)
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
}

//-----------------------------------------------------------------------------

void SignalInfoDockWidget::on_signalTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    quint16 signal_flag = current->data(SignalInfoUtils::SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole).toUInt();
    Q_EMIT selectedSignalChanged(signal_flag);
}

//-----------------------------------------------------------------------------

void SignalInfoDockWidget::on_signalTree_itemClicked(QTreeWidgetItem *item, int)
{
    quint16 signal_flag = item->data(SignalInfoUtils::SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole).toUInt();
    Q_EMIT selectedSignalChanged(signal_flag);
}

} // TiAScope
