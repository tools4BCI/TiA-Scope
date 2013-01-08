#ifndef WELCOME_WIDGET_H
#define WELCOME_WIDGET_H

#include "data_collector/qt_tia_client/qt_tia_client.h"

#include <QWidget>
#include <QSharedPointer>

namespace Ui {
    class WelcomeWidget;
}

namespace TiAScope
{

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = 0);
    ~WelcomeWidget();

Q_SIGNALS:
    void connectionSelected (QSharedPointer<TiAQtImplementation::TiAQtClient> new_client, bool custom_connect);

private Q_SLOTS:
    void on_connectToServer1_clicked ();
    void on_connectToNewServer_clicked ();
    void saveSettings (QString server_ip, unsigned port, bool udp_data_connection);

    void on_customConnectToNewServer_clicked();
    void on_customConnectToServer_clicked();    

    void on_new_udp_toggled(bool checked);

    void on_new_tcp_toggled(bool checked);

    void on_udp_1_toggled(bool checked);

    void on_tcp_1_toggled(bool checked);

private:
    virtual void showEvent (QShowEvent* event);
    void loadSettings ();
    void tryToConnect (QString server_ip, QString port, bool udp_data_connection, bool custom_connect);
    bool clientConnects (QSharedPointer<TiAQtImplementation::TiAQtClient> client, QString server_ip, QString port, bool udp_data_connection, bool custom_connect);
    bool checkAddressString (QString server_ip, QString port) const;

    Ui::WelcomeWidget *ui;
};

}

#endif // WELCOME_WIDGET_H
