#include "connect_dialog.h"
#include "ui_connect_wizard.h"

#include <QSettings>

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
ConnectWizard::ConnectWizard(QWidget *parent) :
    QWizard (parent),
    ui (new Ui::ConnectWizard)
{
    ui->setupUi(this);
    ui->portLineEdit->setValidator (new QIntValidator (0, 0xffff, this));
    ui->connectionPage->setFinalPage (true);

    QSettings settings;
    ui->portLineEdit->setText (settings.value ("connection/port", "9000").toString());
    ui->IPAddressLineEdit->setText (settings.value ("connection/ip", "127.0.0.1").toString());
    ui->udpButton->setChecked (settings.value ("connection/udp_enabled", true).toBool());
}

//-----------------------------------------------------------------------------
ConnectWizard::~ConnectWizard()
{
    delete ui;
}

//-----------------------------------------------------------------------------
QString ConnectWizard::getIPAddress () const
{
    return ui->IPAddressLineEdit->text();
}

//-----------------------------------------------------------------------------
short unsigned ConnectWizard::getPort () const
{
    return ui->portLineEdit->text().toUInt();
}

//-----------------------------------------------------------------------------
bool ConnectWizard::UDPEnabled () const
{
    return ui->udpButton->isChecked ();
}

} } // namespace
