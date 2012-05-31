#include "customize_signalinfo_dialog.h"
#include "ui_customize_signalinfo_dialog.h"
#include "config_widget/signal_info_dock_widget.h"

namespace TiAScope
{

CustomizeSignalInfoDialog::CustomizeSignalInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomizeSignalInfoDialog)
{
    ui->setupUi(this);
}

//-----------------------------------------------------------------------------

CustomizeSignalInfoDialog::~CustomizeSignalInfoDialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------

void CustomizeSignalInfoDialog::initialize()
{
    QSharedPointer<SignalViewSettings> signal_view_settings = QSharedPointer<SignalViewSettings> (new SignalViewSettings);

    SignalInfoDockWidget* signal_info_widget = new SignalInfoDockWidget (signal_view_settings, this);

    signal_info_widget->setFeatures(QDockWidget::NoDockWidgetFeatures);

    meta_info_ = qt_tia_client_->getMetaInfo();

    signal_info_widget->setSignalInfo(meta_info_);

    ui->horizontalLayout->insertWidget(0,signal_info_widget);
//    ui->verticalLayout->addWidget(signal_info_widget);
}

}
