#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
    class MainWindow;
}

namespace tobiss {

class SSClient;

namespace scope {

class SignalInfoDockWidget;
class SubjectInfoDockWidget;
class ViewSettingsDockWidget;
class ApplicationMonitorDockWidget;
class SignalGraphicsScene;
class ReaderThread;
class FourierTransformThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private Q_SLOTS:
    void on_actionConnect_triggered ();
    void on_actionDisconnect_triggered ();

    void ftThreadFinished ();
    void readerThreadFinished ();

private:
    Ui::MainWindow *ui;
    SignalInfoDockWidget* signal_info_widget_;
    SubjectInfoDockWidget* subject_info_widget_;
    ViewSettingsDockWidget* view_settings_widget_;
    ApplicationMonitorDockWidget* monitor_widget_;
    SignalGraphicsScene* graphics_scene_;
    ReaderThread* reader_thread_;
    FourierTransformThread* ft_thread_;
    SSClient* client_;
};

} } // namespace

#endif // MAIN_WINDOW_H
