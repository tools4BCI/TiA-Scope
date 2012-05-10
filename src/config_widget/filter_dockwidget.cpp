#include "filter_dockwidget.h"
#include "ui_filter_dockwidget.h"
#include "data_collector/filters.h"
#include "signal_info_utils.h"

namespace TiAScope
{


//-----------------------------------------------------------------------------
FilterDockWidget::FilterDockWidget (unsigned min_samplingrate, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FilterDockWidget),
    initializing_ (false),
    num_ffts_ (0)
{
    ui->setupUi(this);
    ui->lowpassCutoff->setMaximum (min_samplingrate / 2);
}

//-----------------------------------------------------------------------------
FilterDockWidget::~FilterDockWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------
void FilterDockWidget::setSignalInfo (TiAQtImplementation::TiAMetaInfo const& signal_info)
{
    initializing_ = true;
    ui->signalTree->clear ();
    SignalInfoUtils::addSignalsToTree (ui->signalTree, signal_info, false, false);
    initializing_ = false;
}

//-----------------------------------------------------------------------------
void FilterDockWidget::setSignalInfo (tia::SSConfig const& signal_info)
{
    initializing_ = true;
    ui->signalTree->clear ();
    SignalInfoUtils::addSignalsToTree (ui->signalTree, signal_info, false, false);
    initializing_ = false;
}

//-----------------------------------------------------------------------------
void FilterDockWidget::on_lowpass_toggled (bool checked)
{
    Filters::instance().reset();
    if (checked)
        Filters::instance().appendFilter ("Butterworth", ui->lowpassCutoff->text());
}

//-----------------------------------------------------------------------------
void FilterDockWidget::on_lowpassCutoff_valueChanged (QString const& value)
{
    Filters::instance().reset();
    Filters::instance().appendFilter ("Butterworth", value);
}

//-----------------------------------------------------------------------------
void FilterDockWidget::on_signalTree_itemChanged (QTreeWidgetItem* item, int column)
{
    if (initializing_)
        return;
    if (column == SignalInfoUtils::NAME_COLUMN_INDEX)
    {
        if (item->childCount())
            return;

        unsigned old_num_ffts_ = num_ffts_;

        if (item->checkState(SignalInfoUtils::NAME_COLUMN_INDEX) == Qt::Checked)
            num_ffts_++;
        else
            num_ffts_--;

        if (num_ffts_ == 0)
            Q_EMIT (anyFTEnabled (false));
        else if (old_num_ffts_ == 0)
            Q_EMIT (anyFTEnabled (true));

        Q_EMIT signalChannelFTEnabledChanged (item->data (SignalInfoUtils::SIGNAL_TYPE_COLUMN_INDEX, Qt::UserRole).toUInt(), item->data (SignalInfoUtils::CHANNEL_INDEX_COLUMN_INDEX, Qt::UserRole).toUInt(),
                                              item->checkState(SignalInfoUtils::NAME_COLUMN_INDEX) == Qt::Checked);
    }
}



}
