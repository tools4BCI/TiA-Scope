#ifndef VIEW_SETTINGS_DOCK_WIDGET_H
#define VIEW_SETTINGS_DOCK_WIDGET_H

#include "base/signal_view_settings.h"
#include "base/ft_view_settings.h"

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
    void setFTViewSettings (QSharedPointer<FTViewSettings> settings);

private Q_SLOTS:
    void on_increaseYScaling_clicked ();
    void on_decreaseYScaling_clicked ();
    void on_channelOverlappingSlider_valueChanged (int value);
    void on_fftLogCheckbox_toggled (bool enabled);
    void on_fftUpdateIntervalSpinbox_valueChanged (int value);
    void on_fftWindowSizeSpinbox_valueChanged (double value);
    void on_lowerFrequencyBound_valueChanged (int value);
    void on_upperFrequencyBound_valueChanged (int value);

private:
    Ui::ViewSettingsDockWidget *ui;
    QSharedPointer<SignalViewSettings> signal_view_settings_;
    QSharedPointer<FTViewSettings> ft_view_settings_;
};

} } // namespace

#endif // VIEW_SETTINGS_DOCK_WIDGET_H
