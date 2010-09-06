#ifndef SIGNAL_INFO_DOCK_WIDGET_H
#define SIGNAL_INFO_DOCK_WIDGET_H

#include "base/user_types.h"

#include "config/ss_meta_info.h"

#include <QDockWidget>
#include <QTreeWidgetItem>

namespace Ui
{
    class SignalInfoDockWidget;
}

namespace tobiss { namespace scope {

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

    void setSignalInfo (SignalInfo const& signal_info);

Q_SIGNALS:
    void signalVisibilityChanged (SignalTypeFlag signal, bool visibility);
    void signalChannelFTEnabledChanged (SignalTypeFlag signal, int channel, bool enabled);

private Q_SLOTS:
    void on_signalTree_itemChanged (QTreeWidgetItem* item, int column);

private:
    Ui::SignalInfoDockWidget *ui;
    bool initializing_;
};

} } // namespace

#endif // SIGNAL_INFO_DOCK_WIDGET_H
