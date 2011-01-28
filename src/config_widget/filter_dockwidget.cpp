#include "filter_dockwidget.h"
#include "ui_filter_dockwidget.h"
#include "data_collector/filters.h"

namespace TiAScope
{


//-----------------------------------------------------------------------------
FilterDockWidget::FilterDockWidget (unsigned min_samplingrate, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FilterDockWidget)
{
    ui->setupUi(this);
    ui->lowpassCutoff->setMaximum (min_samplingrate / 2);
}

//-----------------------------------------------------------------------------
FilterDockWidget::~FilterDockWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------
void FilterDockWidget::on_lowpass_toggled (bool checked)
{
    Filters::instance().reset();
    if (checked)
        Filters::instance().appendFilter ("Butterworth", ui->lowpassCutoff->text());
}

//-----------------------------------------------------------------------------
void FilterDockWidget::on_lowpassCutoff_valueChanged (QString const& value)
{
    Filters::instance().reset();
    Filters::instance().appendFilter ("Butterworth", value);
}



}
