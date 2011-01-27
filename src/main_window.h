#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "data_collector/qt_tia_client/qt_tia_client.h"
#include "base/signal_view_settings.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSplitter>

namespace Ui {
    class MainWindow;
}

namespace TiAScope {

class SignalInfoDockWidget;
class SubjectInfoDockWidget;
class ViewSettingsDockWidget;
class ApplicationMonitorDockWidget;
class SignalGraphicsScene;
class SignalGraphicsView;
class ReaderThread;
class FourierTransformThread;
class WelcomeWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private Q_SLOTS:
    void on_actionDisconnect_triggered ();
    void on_actionZoomIn_triggered ();
    void on_actionZoomOut_triggered ();
    void on_actionAutoScaling_toggled (bool checked);
    void on_actionReceiveData_toggled (bool checked);

    void startConnection (QSharedPointer<TiAQtImplementation::TiAQtClient> new_client);

    void ftThreadFinished ();
    void readerThreadFinished ();

private:
    void initWelcomeScreen ();
    void initDataViewScreen ();


    Ui::MainWindow *ui;
//    SignalInfoDockWidget* signal_info_widget_;
//    SubjectInfoDockWidget* subject_info_widget_;
//    ViewSettingsDockWidget* view_settings_widget_;
//    ApplicationMonitorDockWidget* monitor_widget_;
//    SignalGraphicsScene* graphics_scene_;
//    SignalGraphicsScene* fft_graphics_scene_;
    ReaderThread* reader_thread_;
    FourierTransformThread* ft_thread_;
    QSharedPointer<TiAQtImplementation::TiAQtClient> qt_client_;
    QList<QDockWidget*> dock_widgets_;
    QSharedPointer<SignalViewSettings> signal_view_settings_;
//    SignalGraphicsView* view_;
//    SignalGraphicsView* fft_view_;
};

} // TiAScope

#endif // MAIN_WINDOW_H
