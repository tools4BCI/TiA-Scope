#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QWizard>

namespace Ui {
    class ConnectWizard;
}

namespace tobiss {

class TiAClient;

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

private Q_SLOTS:
    void on_ConnectWizard_currentIdChanged (int id);

private:
    Ui::ConnectWizard *ui;

    TiAClient* client_;
};

} } // namespace

#endif // CONNECT_DIALOG_H
