#include "welcome_widget.h"
#include "ui_welcome_widget.h"

#include "data_collector/qt_tia_client/tia_exception.h"
#include "data_collector/qt_tia_client/impl/tia_client_version02.h"
#include "data_collector/qt_tia_client/impl/tia_client_version10.h"
#include "data_collector/qt_tia_client/impl/tia_client_based_libtia.h"


#include <QSettings>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>

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
    tryToConnect (ui->server_ip_1->text(), ui->server_port_1->text(), ui->udp_1->isChecked(),false);
}

//-----------------------------------------------------------------------------
void WelcomeWidget::on_connectToNewServer_clicked ()
{
    tryToConnect (ui->new_server_ip->text(), ui->new_server_port->text(), ui->new_udp->isChecked(),false);
}

//-----------------------------------------------------------------------------

void WelcomeWidget::on_customConnectToServer_clicked()
{
    tryToConnect (ui->server_ip_1->text(), ui->server_port_1->text(), ui->udp_1->isChecked(),true);
}

//-----------------------------------------------------------------------------

void WelcomeWidget::on_customConnectToNewServer_clicked()
{
    tryToConnect (ui->new_server_ip->text(), ui->new_server_port->text(), ui->new_udp->isChecked(),true);
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
void WelcomeWidget::tryToConnect (QString server_ip, QString port, bool udp_data_connection, bool custom_connect)
{
    if (checkAddressString (server_ip, port))
    {

        //use libTia instead of own TiAClientImplementation
        //TODO: add possibility to check if new tia should be used or old one!
//        QSharedPointer<TiAQtImplementation::TiAQtClient> new_client (new TiAQtImplementation::TiAQtClientVersion10);
        QSharedPointer<TiAQtImplementation::TiAQtClient> new_client (new TiAQtImplementation::TiAQtClientBasedLibTiA(true));
        if (!clientConnects (new_client, server_ip, port, udp_data_connection, custom_connect))
        {
            if(!custom_connect)
            {

                qDebug () << "Try Client version 0.2";
                new_client = QSharedPointer<TiAQtImplementation::TiAQtClient> (new TiAQtImplementation::TiAQtClientVersion02);
                if (clientConnects (new_client, server_ip, port, udp_data_connection, custom_connect))
                {
                    Q_EMIT connectionSelected (new_client,false); //due to custom connect is not supported for this version
                    saveSettings (server_ip, port.toUInt(), udp_data_connection);
                }
                else
                    QMessageBox::critical (this, "Error",
                                           QString ("No supported signal server found at ")
                                           .append(server_ip).append(":").append(port));
            }
            else
                QMessageBox::information (this, "Information",
                                          QString ("Version of signal server found at ")
                                          .append(server_ip).append(":").append(port)
                                          .append(" does not support custom configuration!"));
        }
        else
        {
            Q_EMIT connectionSelected (new_client,custom_connect);
            saveSettings (server_ip, port.toUInt(), udp_data_connection);
        }
    }
    else
        QMessageBox::critical (this, "Error", "No valid IP Address or port!");
}

//-----------------------------------------------------------------------------
bool WelcomeWidget::clientConnects (QSharedPointer<TiAQtImplementation::TiAQtClient> client, QString server_ip, QString port, bool udp_data_connection, bool custom_connect)
{
    try
    {
        if(custom_connect)
        {
         TiAQtImplementation::TiAQtClientBasedLibTiA *lib_tia_client = dynamic_cast<TiAQtImplementation::TiAQtClientBasedLibTiA *> (client.data());

         if(lib_tia_client != NULL)
         {
             lib_tia_client->connectToServer(server_ip,port.toUInt());
             lib_tia_client->setDataConnectionType(udp_data_connection);
         }

        }
        else
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

