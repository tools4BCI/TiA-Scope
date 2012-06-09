#include "customize_signalinfo_dialog.h"
#include "ui_customize_signalinfo_dialog.h"
#include "config_widget/signal_info_dock_widget.h"
#include "tia/constants.h"


#include "tia-private/newtia/tia_meta_info_parse_and_build_functions.h"

#include <iostream>

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

    signal_info_widget_ = new SignalInfoDockWidget (signal_view_settings, this);

    signal_info_widget_->setFeatures(QDockWidget::NoDockWidgetFeatures);

    meta_info_ = qt_tia_client_->getMetaInfo();

    signal_info_widget_->setSignalInfo(meta_info_);

    ui->horizontalLayout->insertWidget(0,signal_info_widget_);

}

//-----------------------------------------------------------------------------

void CustomizeSignalInfoDialog::on_buttonBox_accepted()
{
    std::cout << tia::buildTiAMetaInfoXMLString(meta_info_) << std::endl;

    signal_info_widget_->updateSignalInfo(meta_info_);

    //TODO: send meta info to server and wait for response!!!!
//    setResult(QDialog::Rejected);

    std::cout << "-------------------spacer---------------------" << std::endl;

    std::cout << tia::buildTiAMetaInfoXMLString(meta_info_) << std::endl;
}

}

