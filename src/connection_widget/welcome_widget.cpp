#include "welcome_widget.h"
#include "ui_welcome_widget.h"

#include "data_collector/qt_tia_client/tia_exception.h"
#include "data_collector/qt_tia_client/impl/tia_client_version02.h"
#include "data_collector/qt_tia_client/impl/tia_client_version10.h"

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
    tryToConnect (ui->server_ip_1->text(), ui->server_port_1->text(), ui->udp_1->isChecked());
}

//-----------------------------------------------------------------------------
void WelcomeWidget::on_connectToNewServer_clicked ()
{
    tryToConnect (ui->new_server_ip->text(), ui->new_server_port->text(), ui->new_udp->isChecked());
}

//-----------------------------------------------------------------------------
void WelcomeWidget::saveSettings (QString server_ip, unsigned port, bool udp_data_connection)
{
    QSettings settings;
    settings.setValue ("connection/ip", server_ip);
    settings.setValue ("connection/port", port);
    settings.setValue ("connection/udp", udp_data_connection);
}

//-----------------------------------------------------------------------------
void WelcomeWidget::showEvent (QShowEvent* /*event*/)
{
    loadSettings();
}

//-----------------------------------------------------------------------------
void WelcomeWidget::loadSettings ()
{
    QSettings settings;
    ui->server_ip_1->setText (settings.value ("connection/ip", "127.0.0.1").toString());
    ui->server_port_1->setText (settings.value ("connection/port", "9000").toString());
    ui->udp_1->setChecked (settings.value ("connection/udp", true).toBool());
    ui->tcp_1->setChecked (!ui->udp_1->isChecked());
}

//-----------------------------------------------------------------------------
void WelcomeWidget::tryToConnect (QString server_ip, QString port, bool udp_data_connection)
{
    if (checkAddressString (server_ip, port))
    {

        QSharedPointer<TiAQtImplementation::TiAQtClient> new_client (new TiAQtImplementation::TiAQtClientVersion10);
        if (!clientConnects (new_client, server_ip, port, udp_data_connection))
        {
            new_client = QSharedPointer<TiAQtImplementation::TiAQtClient> (new TiAQtImplementation::TiAQtClientVersion02);
            if (clientConnects (new_client, server_ip, port, udp_data_connection))
            {
                Q_EMIT connectionSelected (new_client);
                saveSettings (server_ip, port.toUInt(), udp_data_connection);
            }
            else
                QMessageBox::critical (this, "Error", QString ("No supported signal server found at ").append(server_ip).append(":").append(port));
        }
        else
        {
            Q_EMIT connectionSelected (new_client);
            saveSettings (server_ip, port.toUInt(), udp_data_connection);
        }
    }
    else
        QMessageBox::critical (this, "Error", "No valid IP Address or port!");
}

//-----------------------------------------------------------------------------
bool WelcomeWidget::clientConnects (QSharedPointer<TiAQtImplementation::TiAQtClient> client, QString server_ip, QString port, bool udp_data_connection)
{
    try
    {
        client->connectToServer (server_ip, port.toUInt (), udp_data_connection);
        return true;
    }
    catch (...)
    {
        return false;
    }
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
