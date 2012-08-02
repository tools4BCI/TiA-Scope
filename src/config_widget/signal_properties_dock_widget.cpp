#include "signal_properties_dock_widget.h"
#include "ui_signal_properties_dock_widget.h"

namespace TiAScope
{

SignalPropertiesDockWidget::SignalPropertiesDockWidget(tia::CustomSignal &model
        , tia::Signal const &original_model, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SignalPropertiesDockWidget),
    model_(model),
    original_model_ (original_model)
{
    ui->setupUi(this);    

    QString sr_text = QString::number(original_model_.samplingRate());
    sr_text.append(" Hz");
    ui->lbl_o_sampling_rate->setText(sr_text);

    sr_text = QString::number(model_.samplingRate());
    sr_text.append(" Hz");

    ui->lbl_sampling_rate->setText(sr_text);
    ui->spb_ds_factor->setMaximum(original_model_.samplingRate());

    QString title_text = "Properties of signal type ";
    title_text.append(model_.type().c_str());

    this->setWindowTitle(title_text);

    ui->spb_ds_factor->setValue(model_.DSFactor());
}

//-----------------------------------------------------------------------------

SignalPropertiesDockWidget::~SignalPropertiesDockWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------

void SignalPropertiesDockWidget::on_spb_ds_factor_valueChanged(int value)
{
    quint16 new_sr = original_model_.samplingRate() / value;


    QString sr_text = QString::number(new_sr);
    sr_text.append(" Hz");

    ui->lbl_sampling_rate->setText(sr_text);


//    model_.setSamplingRate(new_sr);
    model_.setDSFactor(value);
}

}
