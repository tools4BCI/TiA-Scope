#include "welcome_widget.h"
#include "ui_welcome_widget.h"

#include "data_collector/qt_tia_client/impl/tia_client_version02.h"

#include <QSettings>
#include <QHostAddress>
#include <QMessageBox>

namespace TiAScope
{

//-----------------------------------------------------------------------------
WelcomeWidget::WelcomeWidget (QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    loadSettings();
}

//-----------------------------------------------------------------------------
WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------
void WelcomeWidget::on_connectToServer1_clicked ()
{
    tryToConnect (ui->server_ip_1->text(), ui->server_port_1->text());
}

//-----------------------------------------------------------------------------
void WelcomeWidget::on_connectToNewServer_clicked ()
{
    tryToConnect (ui->new_server_ip->text(), ui->new_server_port->text());
}

//-----------------------------------------------------------------------------
void WelcomeWidget::saveSettings (QString server_ip, unsigned port)
{
    QSettings settings;
    settings.setValue ("connection/ip", server_ip);
    settings.setValue ("connection/port", port);
}

//-----------------------------------------------------------------------------
void WelcomeWidget::showEvent (QShowEvent* event)
{
    loadSettings();
}

//-----------------------------------------------------------------------------
void WelcomeWidget::loadSettings ()
{
    QSettings settings;
    ui->server_ip_1->setText (settings.value ("connection/ip", "127.0.0.1").toString());
    ui->server_port_1->setText (settings.value ("connection/port", "9000").toString());
}

//-----------------------------------------------------------------------------
void WelcomeWidget::tryToConnect (QString server_ip, QString port)
{
    if (checkAddressString (server_ip, port))
    {
        QSharedPointer<TiAQtImplementation::TiAQtClient> new_client (new TiAQtImplementation::TiAQtClientVersion02);
        try
        {
            new_client->connectToServer (server_ip, port.toUInt ());
            Q_EMIT connectionSelected (new_client);
            saveSettings (server_ip, port.toUInt());
        }
        catch (std::exception &exc)
        {
            QMessageBox::critical (this, "Error", exc.what ());
        }
        catch (...)
        {
            QMessageBox::critical (this, "Error", "Undefined error while connecting!");
        }
    }
    else
        QMessageBox::critical (this, "Error", "No valid IP Address or port!");
}

//-----------------------------------------------------------------------------
bool WelcomeWidget::checkAddressString (QString server_ip, QString port) const
{
    QHostAddress address_tester;
    if (!address_tester.setAddress (server_ip))
        return false;
    bool ok = false;
    port.toUInt (&ok);
    return ok;
}

}
