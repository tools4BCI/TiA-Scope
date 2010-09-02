#include "view_settings_dock_widget.h"
#include "ui_view_settings_dock_widget.h"

namespace tobiss { namespace scope {

ViewSettingsDockWidget::ViewSettingsDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ViewSettingsDockWidget)
{
    ui->setupUi(this);
}

ViewSettingsDockWidget::~ViewSettingsDockWidget()
{
    delete ui;
}

void ViewSettingsDockWidget::setSignalViewSettings (QSharedPointer<SignalViewSettings> settings)
{
    signal_view_settings_ = settings;
    ui->signalVisualisationTimeSpinBox->setValue (signal_view_settings_->getSignalVisualisationTime());
    signal_view_settings_->connect (ui->signalVisualisationTimeSpinBox, SIGNAL(valueChanged(double)), SLOT(setSignalVisualisationTime(double)));

    ui->cyclicModeCheckBox->setChecked (signal_view_settings_->getCyclicMode());
    signal_view_settings_->connect (ui->cyclicModeCheckBox, SIGNAL(toggled(bool)), SLOT(setCyclicMode(bool)));
}

} } // namespace
