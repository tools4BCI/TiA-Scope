#ifndef FILTER_DOCKWIDGET_H
#define FILTER_DOCKWIDGET_H

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

private Q_SLOTS:
    void on_lowpass_toggled (bool checked);
    void on_lowpassCutoff_valueChanged (QString const& text);

private:
    Ui::FilterDockWidget *ui;
};

}

#endif // FILTER_DOCKWIDGET_H
