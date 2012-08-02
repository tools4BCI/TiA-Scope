#ifndef SIGNAL_PROPERTIES_DOCK_WIDGET_H
#define SIGNAL_PROPERTIES_DOCK_WIDGET_H

#include <QDockWidget>
#include "tia/ss_meta_info.h"
#include "tia/custom_signal_info.h"

namespace Ui {
class SignalPropertiesDockWidget;
}

namespace TiAScope
{

class SignalPropertiesDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit SignalPropertiesDockWidget(tia::CustomSignal &model, tia::Signal const &original_model, QWidget *parent = 0);
    ~SignalPropertiesDockWidget();
    
private Q_SLOTS:
    void on_spb_ds_factor_valueChanged(int arg1);

private:
    Ui::SignalPropertiesDockWidget *ui;
    tia::CustomSignal &model_;
    tia::Signal const &original_model_;
};

}

#endif // SIGNAL_PROPERTIES_DOCK_WIDGET_H
