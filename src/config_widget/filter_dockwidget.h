#ifndef FILTER_DOCKWIDGET_H
#define FILTER_DOCKWIDGET_H

#include "base/user_types.h"

#include "tia/ssconfig.h"
#include "tia/defines.h"

#include <QTreeWidgetItem>
#include <QDockWidget>

namespace Ui
{
    class FilterDockWidget;
}

namespace TiAScope
{

class FilterDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit FilterDockWidget (unsigned min_samplingrate, QWidget *parent = 0);
    ~FilterDockWidget();    

    void setSignalInfo (tia::SSConfig const& signal_info);

Q_SIGNALS:
    void signalChannelFTEnabledChanged (SignalTypeFlag signal, int channel, bool enabled);
    void anyFTEnabled (bool enabled);

private Q_SLOTS:
    void on_lowpass_toggled (bool checked);
    void on_lowpassCutoff_valueChanged (QString const& text);
    void on_signalTree_itemChanged (QTreeWidgetItem* item, int column);

    void on_highpassCutoff_currentIndexChanged(int index);

    void on_highpass_toggled(bool arg1);

private:
    Ui::FilterDockWidget *ui;
    bool initializing_;
    unsigned num_ffts_;
    FilterID lowpass_filter_id_, highpass_filter_id_;
};

}

#endif // FILTER_DOCKWIDGET_H
