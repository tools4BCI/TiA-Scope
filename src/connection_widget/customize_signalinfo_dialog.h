#ifndef CUSTOMIZE_SIGNALINFO_DIALOG_H
#define CUSTOMIZE_SIGNALINFO_DIALOG_H

#include <QDialog>
#include "data_collector/qt_tia_client/qt_tia_client.h"
#include "tia/ssconfig.h"


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

    inline void setTiaQtClient(QSharedPointer<TiAQtImplementation::TiAQtClient> qt_tia_client)
    {
        qt_tia_client_ = qt_tia_client;
    }

    void initialize();
    
private:
    Ui::CustomizeSignalInfoDialog *ui;

    QSharedPointer<TiAQtImplementation::TiAQtClient> qt_tia_client_;
    tia::SSConfig meta_info_;

};

}

#endif // CUSTOMIZE_SIGNALINFO_DIALOG_H
