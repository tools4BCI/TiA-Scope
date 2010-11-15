#include "view_settings_dock_widget.h"
#include "ui_view_settings_dock_widget.h"
#include "base/helpers.h"

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
ViewSettingsDockWidget::ViewSettingsDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ViewSettingsDockWidget)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------
ViewSettingsDockWidget::~ViewSettingsDockWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::setSignalViewSettings (QSharedPointer<SignalViewSettings> settings)
{
    signal_view_settings_ = settings;
    ui->signalVisualisationTimeSpinBox->setValue (signal_view_settings_->getSignalVisualisationTime());
    signal_view_settings_->connect (ui->signalVisualisationTimeSpinBox, SIGNAL(valueChanged(double)), SLOT(setSignalVisualisationTime(double)));

    ui->cyclicModeCheckBox->setChecked (signal_view_settings_->getCyclicMode());
    signal_view_settings_->connect (ui->cyclicModeCheckBox, SIGNAL(toggled(bool)), SLOT(setCyclicMode(bool)));
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_increaseYScaling_clicked ()
{
    if (signal_view_settings_.isNull())
        return;
    helpers::animateProperty (signal_view_settings_.data(), "basicYScaling", signal_view_settings_->getBasicYScaling(), signal_view_settings_->getBasicYScaling() * 2);
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_decreaseYScaling_clicked ()
{
    if (signal_view_settings_.isNull())
        return;
    helpers::animateProperty (signal_view_settings_.data(), "basicYScaling", signal_view_settings_->getBasicYScaling(), signal_view_settings_->getBasicYScaling() / 2);
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_channelOverlappingSlider_valueChanged (int value)
{
    if (signal_view_settings_.isNull())
        return;
    signal_view_settings_->setChannelOverlapping (static_cast<float>(value) / 100.0);
//    helpers::animateProperty (signal_view_settings_.data(), "channelOverlapping", signal_view_settings_->getChannelOverlapping(), );
}


} } // namespace
