#ifndef SIGNAL_INFO_DOCK_WIDGET_H
#define SIGNAL_INFO_DOCK_WIDGET_H

#include "base/user_types.h"
#include "base/signal_view_settings.h"

#include "tia/ssconfig.h"

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QSharedPointer>

namespace Ui
{
    class SignalInfoDockWidget;
}

namespace TiAScope {

//-----------------------------------------------------------------------------
/// SignalInfoDockWidget
///
/// a widget displaying signal (and channel) info in a tree widget
class SignalInfoDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SignalInfoDockWidget (QSharedPointer<SignalViewSettings> settings, QWidget *parent = 0);
    ~SignalInfoDockWidget ();  

    void setSignalInfo (tia::SSConfig const& signal_info, bool use_channel_numbers);

    void updateSignalInfo(tia::SSConfig &signal_info);

public Q_SLOTS:
    void clear ();

Q_SIGNALS:
    void channelVisibilityChanged (SignalTypeFlag signal, ChannelID channel, bool visibility);

private Q_SLOTS:
    void on_signalTree_itemChanged (QTreeWidgetItem* item, int column);


private:
    Ui::SignalInfoDockWidget *ui;
    bool initializing_;
    QSharedPointer<SignalViewSettings> settings_;
};

} // TiAScope

#endif // SIGNAL_INFO_DOCK_WIDGET_H
