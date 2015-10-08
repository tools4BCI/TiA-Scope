#include "filter_dockwidget.h"
#include "ui_filter_dockwidget.h"
#include "data_collector/filters.h"
#include "signal_info_utils.h"

#include "data_collector/filters/butterworth_highpass.h"

namespace TiAScope
{


//-----------------------------------------------------------------------------
FilterDockWidget::FilterDockWidget (unsigned min_samplingrate, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FilterDockWidget),
    initializing_ (false),
    num_ffts_ (0),
    lowpass_filter_id_ (0),
    highpass_filter_id_ (0)
{
    ui->setupUi(this);
    ui->lowpassCutoff->setMaximum (min_samplingrate / 2);

    // read out possible normalized frequencies of the highpass filter
    // and transform them into frequencies in (Hz)
    for (int itemIdx = 0; itemIdx < ButterworthHighpass::NSPECS; ++itemIdx) {
        ui->highpassCutoff->addItem(QString::number(ButterworthHighpass::NORMFREQS[itemIdx]*min_samplingrate/2,'g',3));
    }

    Filters::instance().reset();
}

//-----------------------------------------------------------------------------
FilterDockWidget::~FilterDockWidget()
{
    delete ui;
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

    if (lowpass_filter_id_ > 0)
        Filters::instance().removeFilter(lowpass_filter_id_);

    if (checked) {

        lowpass_filter_id_ = Filters::instance().appendFilter ("Butterworth", ui->lowpassCutoff->text());
    }
}

//-----------------------------------------------------------------------------
void FilterDockWidget::on_lowpassCutoff_valueChanged (QString const& value)
{    
    if (lowpass_filter_id_ > 0)
        Filters::instance().removeFilter(lowpass_filter_id_);
    lowpass_filter_id_ = Filters::instance().appendFilter ("Butterworth", value);
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

//-----------------------------------------------------------------------------
void TiAScope::FilterDockWidget::on_highpass_toggled(bool checked)
{

    if (highpass_filter_id_ > 0)
        Filters::instance().removeFilter(highpass_filter_id_);

    if (checked){

        highpass_filter_id_ = Filters::instance().appendFilter ("ButterworthHighpass", QString::number(ui->highpassCutoff->currentIndex()));
    }
}

//-----------------------------------------------------------------------------
void TiAScope::FilterDockWidget::on_highpassCutoff_currentIndexChanged(int index)
{

    if (highpass_filter_id_ > 0)
        Filters::instance().removeFilter(highpass_filter_id_);

    highpass_filter_id_ = Filters::instance().appendFilter ("ButterworthHighpass", QString::number(index));
}
