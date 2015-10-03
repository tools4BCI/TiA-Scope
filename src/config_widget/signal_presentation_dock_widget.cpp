#include "signal_presentation_dock_widget.h"
#include "ui_signal_presentation_dock_widget.h"
#include "signal_info_utils.h"

#include <QDebug>

namespace TiAScope {

SignalPresentationDockWidget::SignalPresentationDockWidget(QSharedPointer<SignalViewSettings> settings, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SignalPresentationDockWidget),
    initializing_ (false),
    settings_ (settings)
{
    ui->setupUi(this);

    signal_info_widget_ = new SignalInfoDockWidget(settings_,this);

    signal_info_widget_->setFeatures(QDockWidget::NoDockWidgetFeatures);

    //pass signal from SignalInfoDockWidget on
    connect(signal_info_widget_, SIGNAL(channelVisibilityChanged(SignalTypeFlag,ChannelID,bool)),
                 this, SIGNAL(channelVisibilityChanged(SignalTypeFlag,ChannelID,bool)));

    ((QVBoxLayout *)ui->dockWidgetContents->layout())->insertWidget(3,signal_info_widget_);
}

//-----------------------------------------------------------------------------

SignalPresentationDockWidget::~SignalPresentationDockWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------

void SignalPresentationDockWidget::setSignalInfo (tia::SSConfig const& signal_info)
{
    signal_info_widget_->setSignalInfo(signal_info, false);
}

//-----------------------------------------------------------------------------
void SignalPresentationDockWidget::clear ()
{
    signal_info_widget_->clear();
}

//-------------------------------------------------------------------------------------------------
void SignalPresentationDockWidget::on_channelOverlappingSlider_valueChanged (int value)
{
    if (settings_.isNull())
        return;
    settings_->setChannelOverlapping (static_cast<float>(value) / 100.0);
}

//-------------------------------------------------------------------------------------------------
void SignalPresentationDockWidget::on_cyclicMode_toggled (bool checked)
{
    settings_->setCyclicMode (checked);
}

} // TiAScope

//-------------------------------------------------------------------------------------------------
void TiAScope::SignalPresentationDockWidget::on_spinBoxVisTime_valueChanged(int value)
{
    settings_->setSignalVisualisationTime(value);
}
