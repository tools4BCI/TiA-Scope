#ifndef APPLICATION_MONITOR_DOCK_WIDGET_H
#define APPLICATION_MONITOR_DOCK_WIDGET_H

#include <QDockWidget>
#include <QList>
#include <QMutex>

namespace Ui {
    class ApplicationMonitorDockWidget;
}

namespace tobiss { namespace scope {


class ApplicationMonitorDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ApplicationMonitorDockWidget (QWidget *parent = 0);
    ~ApplicationMonitorDockWidget();

public Q_SLOTS:
    //-------------------------------------------------------------------------------------------------
    /// displays the name of the object in a list
    /// removes the object from that list if the object is destroyed
    void addObjectLifeMonitor (QObject* object);

    //-------------------------------------------------------------------------------------------------
    /// handles different types of debug output and prints it into a textbox
    static void debugMessaging (QtMsgType type, const char* message);

private Q_SLOTS:
    void objectDestroyed (QObject* object);

private:
    static QList<ApplicationMonitorDockWidget*> instances_;
    Ui::ApplicationMonitorDockWidget *ui;
    static QMutex mutex_;
};

} } // namespace

#endif // APPLICATION_MONITOR_DOCK_WIDGET_H
