#ifndef VIEW_SETTINGS_DOCK_WIDGET_H
#define VIEW_SETTINGS_DOCK_WIDGET_H

#include "base/signal_view_settings.h"

#include <QDockWidget>

namespace Ui {
    class ViewSettingsDockWidget;
}

namespace tobiss { namespace scope {



class ViewSettingsDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ViewSettingsDockWidget(QWidget *parent = 0);
    ~ViewSettingsDockWidget();

    void setSignalViewSettings (QSharedPointer<SignalViewSettings> settings);

private Q_SLOTS:
    void on_increaseYScaling_clicked ();
    void on_decreaseYScaling_clicked ();

private:
    Ui::ViewSettingsDockWidget *ui;
    QSharedPointer<SignalViewSettings> signal_view_settings_;
};

} } // namespace

#endif // VIEW_SETTINGS_DOCK_WIDGET_H
