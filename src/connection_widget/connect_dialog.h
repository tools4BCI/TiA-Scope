#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QWizard>

namespace Ui {
    class ConnectWizard;
}

namespace tobiss {

class TiAClient;
}

namespace TiAScope {

//-----------------------------------------------------------------------------
/// ConnectDialog
class ConnectWizard : public QWizard
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------
    static void saveSettings (ConnectWizard const& connect_wizard);

    //-------------------------------------------------------------------------
    explicit ConnectWizard (QWidget *parent = 0);

    //-------------------------------------------------------------------------
    virtual ~ConnectWizard ();

    //-------------------------------------------------------------------------
    QString getIPAddress () const;

    //-------------------------------------------------------------------------
    short unsigned getPort () const;

    //-------------------------------------------------------------------------
    bool UDPEnabled () const;

private Q_SLOTS:
    void on_ConnectWizard_currentIdChanged (int id);

private:
    Ui::ConnectWizard *ui;

    tobiss::TiAClient* client_;
};

} // TiAScope

#endif // CONNECT_DIALOG_H
