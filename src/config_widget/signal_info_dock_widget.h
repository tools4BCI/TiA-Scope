#ifndef SIGNAL_INFO_DOCK_WIDGET_H
#define SIGNAL_INFO_DOCK_WIDGET_H

#include "base/user_types.h"

#include "tia/ss_meta_info.h"

#include <QDockWidget>
#include <QTreeWidgetItem>

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
    explicit SignalInfoDockWidget (QWidget *parent = 0);
    ~SignalInfoDockWidget ();

    void setSignalInfo (tobiss::SignalInfo const& signal_info);

public Q_SLOTS:
    void clear ();

Q_SIGNALS:
    void channelVisibilityChanged (SignalTypeFlag signal, ChannelID channel, bool visibility);
    void signalChannelFTEnabledChanged (SignalTypeFlag signal, int channel, bool enabled);

private Q_SLOTS:
    void on_signalTree_itemChanged (QTreeWidgetItem* item, int column);

private:
    Ui::SignalInfoDockWidget *ui;
    bool initializing_;
};

} // TiAScope

#endif // SIGNAL_INFO_DOCK_WIDGET_H
