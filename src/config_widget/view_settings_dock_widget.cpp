#include "view_settings_dock_widget.h"
#include "ui_view_settings_dock_widget.h"
#include "base/helpers.h"
#include "data_collector/filters.h"

#include <QSettings>

namespace TiAScope {

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
void ViewSettingsDockWidget::setFTViewSettings (QSharedPointer<FTViewSettings> settings)
{
    ft_view_settings_ = settings;
    ui->lowerFrequencyBound->setValue (ft_view_settings_->lowerFrequenceBound());
    ui->upperFrequencyBound->setValue (ft_view_settings_->upperFrequenceBound());
    ui->lowerFrequencyBound->setMaximum (ft_view_settings_->maxSamplingRate() / 2);
    ui->upperFrequencyBound->setMaximum (ft_view_settings_->maxSamplingRate() / 2);

    ft_view_settings_->connect (ui->lowerFrequencyBound, SIGNAL(valueChanged(int)), SLOT(setLowerFrequenceBound(int)));
    ft_view_settings_->connect (ui->upperFrequencyBound, SIGNAL(valueChanged(int)), SLOT(setUpperFrequenceBound(int)));
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
void ViewSettingsDockWidget::on_lowPassNo_toggled (bool enabled)
{
    if (!enabled)
        return;
    Filters::instance().reset();
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_lowPass100_toggled (bool enabled)
{
    if (!enabled)
        return;
    Filters::instance().reset();
    Filters::instance().appendFilter ("Butterworth", "100");
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_lowPass150_toggled (bool enabled)
{
    if (!enabled)
        return;
    Filters::instance().reset();
    Filters::instance().appendFilter ("Butterworth", "150");
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_lowPass200_toggled (bool enabled)
{
    if (!enabled)
        return;
    Filters::instance().reset();
    Filters::instance().appendFilter ("Butterworth", "200");
}


//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_channelOverlappingSlider_valueChanged (int value)
{
    if (signal_view_settings_.isNull())
        return;
    signal_view_settings_->setChannelOverlapping (static_cast<float>(value) / 100.0);
//    helpers::animateProperty (signal_view_settings_.data(), "channelOverlapping", signal_view_settings_->getChannelOverlapping(), );
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_fftLogCheckbox_toggled (bool enabled)
{
    QSettings settings;
    settings.setValue ("fourier/ln", enabled);
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_fftUpdateIntervalSpinbox_valueChanged (int value)
{
    QSettings settings;
    settings.setValue ("fourier/update_interval_msec", value);
}

//-------------------------------------------------------------------------------------------------
void ViewSettingsDockWidget::on_fftWindowSizeSpinbox_valueChanged (double value)
{
    QSettings settings;
    settings.setValue ("fourier/window_size_s", value);
}





} // TiAScope
