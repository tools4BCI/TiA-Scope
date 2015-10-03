#ifndef SIGNAL_PRESENTATION_DOCK_WIDGET_H
#define SIGNAL_PRESENTATION_DOCK_WIDGET_H

#include "base/user_types.h"
#include "base/signal_view_settings.h"

#include "tia/ssconfig.h"

#include "signal_info_dock_widget.h"

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QSharedPointer>

namespace Ui {
class SignalPresentationDockWidget;
}

namespace TiAScope
{

class SignalPresentationDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit SignalPresentationDockWidget(QSharedPointer<SignalViewSettings> settings, QWidget *parent = 0);
    ~SignalPresentationDockWidget();

    void setSignalInfo (tia::SSConfig const& signal_info);

public Q_SLOTS:
    void clear ();

Q_SIGNALS:
    void channelVisibilityChanged (SignalTypeFlag signal, ChannelID channel, bool visibility);

private Q_SLOTS:
    void on_channelOverlappingSlider_valueChanged (int value);
    void on_cyclicMode_toggled (bool checked);

    void on_spinBoxVisTime_valueChanged(int arg1);

private:
    Ui::SignalPresentationDockWidget *ui;
    SignalInfoDockWidget *signal_info_widget_;
    bool initializing_;
    QSharedPointer<SignalViewSettings> settings_;
    
};

}

#endif // SIGNAL_PRESENTATION_DOCK_WIDGET_H
