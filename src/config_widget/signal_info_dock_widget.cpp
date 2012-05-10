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
void SignalInfoDockWidget::setSignalInfo (TiAQtImplementation::TiAMetaInfo const& signal_info)
{
    initializing_ = true;
    ui->signalTree->clear ();
    SignalInfoUtils::addSignalsToTree (ui->signalTree, signal_info);
    initializing_ = false;
}
//-----------------------------------------------------------------------------
//bad hack to undefine signals that is used by Qt
//but at the same time defines a method of SignalInfo
//used in libTiA
#undef signals

void SignalInfoDockWidget::setSignalInfo (tia::SSConfig const& signal_info)
{
    initializing_ = true;
    ui->signalTree->clear ();
    SignalInfoUtils::addSignalsToTree (ui->signalTree, signal_info);
    initializing_ = false;
}

//revert hack that signals is undefined
#define signals Q_SIGNALS

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

//-------------------------------------------------------------------------------------------------
void SignalInfoDockWidget::on_channelOverlappingSlider_valueChanged (int value)
{
    if (settings_.isNull())
        return;
    settings_->setChannelOverlapping (static_cast<float>(value) / 100.0);
}

//-------------------------------------------------------------------------------------------------
void SignalInfoDockWidget::on_cyclicMode_toggled (bool checked)
{
    settings_->setCyclicMode (checked);
}


} // TiAScope
