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

    signal_info_widget_ = new SignalInfoDockWidget (signal_view_settings, this);

    signal_info_widget_->setFeatures(QDockWidget::NoDockWidgetFeatures);

    meta_info_ = qt_tia_client_->getMetaInfo();

    signal_info_widget_->setSignalInfo(meta_info_, true);

    ui->horizontalLayout->insertWidget(0,signal_info_widget_);


    ui->errorLabel->setVisible(false);
    ui->horizontal_line->setVisible(false);
}

//-----------------------------------------------------------------------------

bool CustomizeSignalInfoDialog::validateDialog(QString &error_container)
{    

    tia::SSConfig meta_info_copy = meta_info_;

    signal_info_widget_->updateSignalInfo(meta_info_copy);

    std::string std_error_container;

    bool server_result = qt_tia_client_->trySetCustomSignalInfo(
                meta_info_copy.signal_info, std_error_container);

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
//            if(error_container.length() == 0)
//                error_container = "";

//            QMessageBox::information (this, "Information",
//                                      QString ("Server rejected custom signal info! Details: ").append(error_container));

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

}

//-----------------------------------------------------------------------------

void TiAScope::CustomizeSignalInfoDialog::on_buttonBox_accepted()
{
    done(QDialog::Accepted);
}

//-----------------------------------------------------------------------------

void TiAScope::CustomizeSignalInfoDialog::on_buttonBox_rejected()
{
    done(QDialog::Rejected);
}
