#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QWizard>

namespace Ui {
    class ConnectWizard;
}

namespace tobiss {

class SSClient;

namespace scope {

//-----------------------------------------------------------------------------
/// ConnectDialog
class ConnectWizard : public QWizard
{
    Q_OBJECT

public:
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

private:
    Ui::ConnectWizard *ui;

    SSClient* client_;
};

} } // namespace

#endif // CONNECT_DIALOG_H
