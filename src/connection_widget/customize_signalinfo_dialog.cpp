#include "customize_signalinfo_dialog.h"
#include "ui_customize_signalinfo_dialog.h"
#include "config_widget/signal_info_dock_widget.h"
#include "tia/constants.h"


#include <QMessageBox>
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

    original_meta_info_ = qt_tia_client_->getMetaInfo();

    custom_signal_info_ = qt_tia_client_->getConfigAsCustomConfig();

    signal_info_widget_ = new SignalInfoDockWidget (signal_view_settings, this);     
    signal_info_widget_->setFeatures(QDockWidget::NoDockWidgetFeatures);    

    ui->horizontalLayout->insertWidget(0,signal_info_widget_);

    signal_properties_widget_ = NULL;

    connect(signal_info_widget_,SIGNAL(selectedSignalChanged(SignalTypeFlag)),this,SLOT(signal_info_widget_selectedSignalChanged(SignalTypeFlag)));

    signal_info_widget_->setSignalInfo(original_meta_info_, true);

    ui->errorLabel->setVisible(false);
    ui->horizontal_line->setVisible(false);
}

//-----------------------------------------------------------------------------

bool CustomizeSignalInfoDialog::validateDialog(QString &error_container)
{    

    tia::CustomSignalInfo *tmp = new tia::CustomSignalInfo(*(custom_signal_info_.get()));

    tia::CustomSignalInfoPtr tmp_ptr (tmp);

    signal_info_widget_->updateSignalInfo(tmp_ptr);

    std::string std_error_container;

    bool server_result = qt_tia_client_->trySetCustomSignalInfo(
                tmp_ptr, std_error_container);

    error_container = std_error_container.c_str();        

    if(server_result == false)
    {        
        return false;
    }
    else
    {
        //if the server was able to accept our customized signal info
        //request the updated meta_info from the server
        qt_tia_client_->requestMetaInfo();
        return true;
    }

}

//-----------------------------------------------------------------------------

void CustomizeSignalInfoDialog::showError(QString error_msg)
{
    ui->errorLabel->setText(error_msg);
    ui->errorLabel->setVisible(true);
    ui->horizontal_line->setVisible(true);
}

//-----------------------------------------------------------------------------

void CustomizeSignalInfoDialog::done(int result)
{
    if(QDialog::Accepted == result) // ok was pressed
    {
        QString error_container;

        if(validateDialog(error_container)) // validate the data somehow
        {
            QDialog::done(result);
            return;
        }
        else
        {
            showError(error_container);
            return;
        }
    }
    else // cancel, close or exc was pressed
    {
        QDialog::done(result);
        return;
    }
}

//-----------------------------------------------------------------------------

void CustomizeSignalInfoDialog::on_buttonBox_accepted()
{
    done(QDialog::Accepted);
}

//-----------------------------------------------------------------------------

void CustomizeSignalInfoDialog::on_buttonBox_rejected()
{
    done(QDialog::Rejected);
}

//-----------------------------------------------------------------------------

void CustomizeSignalInfoDialog::signal_info_widget_selectedSignalChanged(SignalTypeFlag signal)
{
    tia::Constants constants;
    qDebug("Selection changed to signal type: ");
    qDebug(constants.getSignalName(signal).c_str());
    qDebug("\n");    

    tia::CustomSignal &model = custom_signal_info_->signals()[constants.getSignalName(signal)];

    tia::Signal const &o_model = original_meta_info_.signal_info.signals()[constants.getSignalName(signal)];

    if(signal_properties_widget_)
    {
        ui->horizontalLayout->removeWidget(signal_properties_widget_);
        delete signal_properties_widget_;
        signal_properties_widget_ = NULL;
    }

    signal_properties_widget_ = new SignalPropertiesDockWidget(model, o_model, this);
    signal_properties_widget_->setFeatures(QDockWidget::NoDockWidgetFeatures);

    ui->horizontalLayout->insertWidget(2,signal_properties_widget_);
}

}
