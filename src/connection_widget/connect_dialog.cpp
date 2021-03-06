#include "connect_dialog.h"
#include "ui_connect_wizard.h"

#include <QSettings>
#include <QMessageBox>

namespace TiAScope {

//-------------------------------------------------------------------------
void ConnectWizard::saveSettings (ConnectWizard const& connect_wizard)
{
    QSettings settings;
    settings.setValue ("connection/ip", connect_wizard.getIPAddress());
    settings.setValue ("connection/port", connect_wizard.getPort());
    settings.setValue ("connection/udp_enabled", connect_wizard.UDPEnabled());
}

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

    connect(this, SIGNAL(currentIdChanged(int)), SLOT(on_ConnectWizard_currentIdChanged(int)));
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

//-----------------------------------------------------------------------------
void ConnectWizard::on_ConnectWizard_currentIdChanged (int id)
{
    switch (id)
    {
    case 1:
//        if (client_->connected ())
//        {
//            ui->signalTree->clear();
//            client_->disconnect ();
//        }
//        try
//        {
//            client_->connect (ui->IPAddressLineEdit->text().toStdString(), ui->portLineEdit->text().toUInt());
//            client_->requestConfig ();
//            tobiss::SSConfig config = client_->config ();
//            for (tobiss::SignalInfo::SignalMap::const_iterator signal_iter = config.signal_info.signals().begin ();
//                 signal_iter != config.signal_info.signals().end ();
//                 ++signal_iter)
//            {
//                QTreeWidgetItem* signal_item = new QTreeWidgetItem (ui->signalTree);
//                signal_item->setText (0, signal_iter->first.c_str());
//                Q_FOREACH (tobiss::Channel channel, signal_iter->second.channels())
//                {
//                    QTreeWidgetItem* channel_item = new QTreeWidgetItem (signal_item);
//                    channel_item->setText (0, channel.id().c_str());
//                }
//            }
//        }
//        catch (std::ios_base::failure &exception)
//        {
//            QMessageBox::critical (0, "Failed", QString("Connection could not be established!\n").append(exception.what()));
//        }
        break;
    }
}


} // TiAScope
