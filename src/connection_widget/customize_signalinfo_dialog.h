#ifndef CUSTOMIZE_SIGNALINFO_DIALOG_H
#define CUSTOMIZE_SIGNALINFO_DIALOG_H

#include <QDialog>
#include "data_collector/qt_tia_client/impl/tia_client_based_libtia.h"
#include "tia/ssconfig.h"
#include "base/user_types.h"
#include "config_widget/signal_info_dock_widget.h"


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

    inline tia::SSConfig getCustomMetaInfo()
    {
        return meta_info_;
    }

private:
    bool validateDialog(QString &error_container);
    Ui::CustomizeSignalInfoDialog *ui;

    SignalInfoDockWidget* signal_info_widget_;

    QSharedPointer<TiAQtImplementation::TiAQtClientBasedLibTiA> qt_tia_client_;
    tia::SSConfig meta_info_;

    void showError(QString error_msg);

public Q_SLOTS:
    virtual void done(int result);

private Q_SLOTS:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

}

#endif // CUSTOMIZE_SIGNALINFO_DIALOG_H
