#include "connect_dialog.h"
#include "ui_connect_dialog.h"

#include <QSettings>

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    ui->portLineEdit->setValidator (new QIntValidator (0, 0xffff, this));

    QSettings settings;
    ui->portLineEdit->setText (settings.value ("connection/port", "9000").toString());
    ui->IPAddressLineEdit->setText (settings.value ("connection/ip", "127.0.0.1").toString());
    ui->udpButton->setChecked (settings.value ("connection/udp_enabled", true).toBool());
}

//-----------------------------------------------------------------------------
ConnectDialog::~ConnectDialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------
QString ConnectDialog::getIPAddress () const
{
    return ui->IPAddressLineEdit->text();
}

//-----------------------------------------------------------------------------
short unsigned ConnectDialog::getPort () const
{
    return ui->portLineEdit->text().toUInt();
}

//-----------------------------------------------------------------------------
bool ConnectDialog::UDPEnabled () const
{
    return ui->udpButton->isChecked ();
}

} } // namespace
