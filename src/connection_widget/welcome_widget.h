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
    void connectionSelected (QSharedPointer<TiAQtImplementation::TiAQtClient> new_client);

private Q_SLOTS:
    void on_connectToServer1_clicked ();
    void on_connectToNewServer_clicked ();
    void saveSettings (QString server_ip, unsigned port, bool udp_data_connection);

private:
    virtual void showEvent (QShowEvent* event);
    void loadSettings ();
    void tryToConnect (QString server_ip, QString port, bool udp_data_connection);
    bool clientConnects (QSharedPointer<TiAQtImplementation::TiAQtClient> client, QString server_ip, QString port, bool udp_data_connection);
    bool checkAddressString (QString server_ip, QString port) const;

    Ui::WelcomeWidget *ui;
};

}

#endif // WELCOME_WIDGET_H
