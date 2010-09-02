#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QDialog>

namespace Ui {
    class ConnectDialog;
}

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
/// ConnectDialog
class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------
    explicit ConnectDialog (QWidget *parent = 0);

    //-------------------------------------------------------------------------
    virtual ~ConnectDialog ();

    //-------------------------------------------------------------------------
    QString getIPAddress () const;

    //-------------------------------------------------------------------------
    short unsigned getPort () const;

    //-------------------------------------------------------------------------
    bool UDPEnabled () const;

private:
    Ui::ConnectDialog *ui;
};

} } // namespace

#endif // CONNECT_DIALOG_H
