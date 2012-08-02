#ifndef CUSTOMIZE_SIGNALINFO_DIALOG_H
#define CUSTOMIZE_SIGNALINFO_DIALOG_H

#include <QDialog>
#include "data_collector/qt_tia_client/impl/tia_client_based_libtia.h"
#include "tia/ssconfig.h"
#include "base/user_types.h"
#include "config_widget/signal_info_dock_widget.h"
#include "config_widget/signal_properties_dock_widget.h"


namespace Ui {
class CustomizeSignalInfoDialog;
}

namespace TiAScope
{

class CustomizeSignalInfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CustomizeSignalInfoDialog(QWidget *parent = 0);
    ~CustomizeSignalInfoDialog();

    inline void setTiaQtClient(QSharedPointer<TiAQtImplementation::TiAQtClientBasedLibTiA> qt_tia_client)
    {
        qt_tia_client_ = qt_tia_client;
    }

    void initialize();

private:
    bool validateDialog(QString &error_container);
    Ui::CustomizeSignalInfoDialog *ui;

    SignalInfoDockWidget* signal_info_widget_;
    SignalPropertiesDockWidget *signal_properties_widget_;

    QSharedPointer<TiAQtImplementation::TiAQtClientBasedLibTiA> qt_tia_client_;
    tia::CustomSignalInfoPtr custom_signal_info_;
    tia::SSConfig original_meta_info_;

    void showError(QString error_msg);

public Q_SLOTS:
    virtual void done(int result);

private Q_SLOTS:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void signal_info_widget_selectedSignalChanged (SignalTypeFlag signal);
};

}

#endif // CUSTOMIZE_SIGNALINFO_DIALOG_H
