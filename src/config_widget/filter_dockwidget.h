#ifndef FILTER_DOCKWIDGET_H
#define FILTER_DOCKWIDGET_H

#include "data_collector/qt_tia_client/tia_metainfo.h"
#include "base/user_types.h"

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

    void setSignalInfo (TiAQtImplementation::TiAMetaInfo const& signal_info);

Q_SIGNALS:
    void signalChannelFTEnabledChanged (SignalTypeFlag signal, int channel, bool enabled);
    void anyFTEnabled (bool enabled);

private Q_SLOTS:
    void on_lowpass_toggled (bool checked);
    void on_lowpassCutoff_valueChanged (QString const& text);
    void on_signalTree_itemChanged (QTreeWidgetItem* item, int column);

private:
    Ui::FilterDockWidget *ui;
    bool initializing_;
    unsigned num_ffts_;
};

}

#endif // FILTER_DOCKWIDGET_H
