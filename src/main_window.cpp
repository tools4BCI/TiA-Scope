#include "main_window.h"
#include "ui_main_window.h"

#include "base/data_buffer.h"
#include "base/signal_view_settings.h"
#include "base/helpers.h"

#include "visualisation/signal_graphics_object.h"
#include "visualisation/signal_graphics_scene.h"
#include "visualisation/signal_graphics_view.h"

#include "config_widget/signal_info_dock_widget.h"
#include "config_widget/subject_info_dock_widget.h"
#include "config_widget/view_settings_dock_widget.h"
#include "config_widget/application_monitor_dock_widget.h"

#include "connection_widget/connect_dialog.h"

#include "data_collector/reader_thread.h"
#include "data_collector/fourier_transform_thread.h"

#include "tia/ssclient.h"
#include "tia/ssconfig.h"
#include "tia/ss_meta_info.h"

#include <QCoreApplication>
#include <QSharedPointer>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QDebug>

#include <iostream>

namespace tobiss { namespace scope {

namespace MainWindowHelper
{
    inline void monitorObjectLife (ApplicationMonitorDockWidget* monitor, QObject* object);
}

//-----------------------------------------------------------------------------
MainWindow::MainWindow (QWidget *parent) :
    QMainWindow (parent),
    ui (new Ui::MainWindow),
    signal_info_widget_ (0),
    subject_info_widget_ (0),
    monitor_widget_ (0),
    graphics_scene_ (0),
    reader_thread_ (0),
    ft_thread_ (0),
    view_ (0)
{
    ui->setupUi(this);
    view_ = new SignalGraphicsView (this);
    this->setCentralWidget (view_);

    view_settings_widget_ = new ViewSettingsDockWidget (this);
    addDockWidget (Qt::LeftDockWidgetArea, view_settings_widget_);

    signal_info_widget_ = new SignalInfoDockWidget (this);
    addDockWidget (Qt::RightDockWidgetArea, signal_info_widget_);

    subject_info_widget_ = new SubjectInfoDockWidget (this);
    addDockWidget (Qt::RightDockWidgetArea, subject_info_widget_);

    if (QCoreApplication::arguments().contains ("--test") || QCoreApplication::arguments().contains("-t"))
    {
        monitor_widget_ = new ApplicationMonitorDockWidget (this);
        qInstallMsgHandler (ApplicationMonitorDockWidget::debugMessaging);
        addDockWidget (Qt::LeftDockWidgetArea, monitor_widget_);
    }
    view_->setInteractive (true);
}

//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//-----------------------------------------------------------------------------
void MainWindow::on_actionConnect_triggered ()
{
    qDebug () << metaObject()->className() << "." << __FUNCTION__;

    ConnectWizard connection_dialog;
    if (connection_dialog.exec() != QDialog::Accepted)
        return;

    QSettings settings;
    settings.setValue ("connection/ip", connection_dialog.getIPAddress());
    settings.setValue ("connection/port", connection_dialog.getPort());
    settings.setValue ("connection/udp_enabled", connection_dialog.UDPEnabled());

    QSharedPointer<SignalViewSettings> signal_view_settings (new SignalViewSettings);

    client_ = new tobiss::SSClient ();
    try
    {
        client_->connect (connection_dialog.getIPAddress().toStdString(), connection_dialog.getPort());
        client_->requestConfig ();
        SSConfig config = client_->config();
        signal_info_widget_->setSignalInfo (config.signal_info);
        signal_view_settings->connect (signal_info_widget_, SIGNAL(channelVisibilityChanged(SignalTypeFlag,ChannelID,bool)), SLOT(setChannelVisibility(SignalTypeFlag,ChannelID,bool)));

        subject_info_widget_->setSubjectInfo (config.subject_info);
        view_settings_widget_->setSignalViewSettings (signal_view_settings);
        QSharedPointer<DataBuffer> data_buffer (new DataBuffer (config.signal_info.signals (), 5));
        MainWindowHelper::monitorObjectLife (monitor_widget_, data_buffer.data());

        ft_thread_ =  new FourierTransformThread (data_buffer, this);
        MainWindowHelper::monitorObjectLife (monitor_widget_, ft_thread_);

        graphics_scene_ = new SignalGraphicsScene (this);
        graphics_scene_->connect (view_, SIGNAL(widthChanged(int)), SLOT(setSceneRectWidth(int)));
        MainWindowHelper::monitorObjectLife (monitor_widget_, graphics_scene_);
        ft_thread_->connect (signal_info_widget_, SIGNAL(signalChannelFTEnabledChanged(SignalTypeFlag,int,bool)), SLOT(enableFT(SignalTypeFlag,int,bool)), Qt::QueuedConnection);

        for (SignalInfo::SignalMap::const_iterator signal_iter = config.signal_info.signals().begin ();
             signal_iter != config.signal_info.signals().end ();
             ++signal_iter)
        {
            SignalGraphicsObject* signal_object = new SignalGraphicsObject (signal_iter->second, data_buffer, signal_view_settings, ft_thread_);
            signal_object->setWidth (view_->width());
            signal_object->connect (view_, SIGNAL(widthChanged(int)), SLOT(setWidth(int)));
            graphics_scene_->addSignalGraphicsObject (TypeConverter::stdStringToSignalTypeFlag (signal_iter->second.type()), signal_object);
        }
        view_->setScene (graphics_scene_);
        graphics_scene_->startTimer (40);

        reader_thread_ = new ReaderThread (data_buffer, client_, connection_dialog.UDPEnabled(), this);
        MainWindowHelper::monitorObjectLife (monitor_widget_, reader_thread_);

        ui->actionConnect->setEnabled (false);
        ui->actionDisconnect->setEnabled (true);
        ui->actionReceiveData->setEnabled (true);
    }
    catch (std::ios_base::failure &exception)
    {
        QMessageBox::critical (0, "Failed", QString("Connection could not be established!\n").append(exception.what()));
    }
}


//-----------------------------------------------------------------------------
void MainWindow::on_actionDisconnect_triggered ()
{
    connect (ft_thread_, SIGNAL(finished()), SLOT(ftThreadFinished()));
    connect (reader_thread_, SIGNAL(finished()), SLOT(readerThreadFinished()));
    ui->actionReceiveData->setChecked (false);

    if (ft_thread_->isFinished())
        ftThreadFinished();
    if (reader_thread_->isFinished())
        readerThreadFinished();
    ui->actionConnect->setEnabled (true);
    ui->actionDisconnect->setEnabled (false);
    ui->actionReceiveData->setEnabled (false);
    delete graphics_scene_;
    graphics_scene_ = 0;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::on_actionReceiveData_toggled (bool checked)
{
    if (checked)
    {
        reader_thread_->start ();
        ft_thread_->start ();
    }
    else
    {
        reader_thread_->stop();
        ft_thread_->stop();
    }
}

//-----------------------------------------------------------------------------
void MainWindow::ftThreadFinished ()
{
    qDebug () << metaObject()->className() << "." << __FUNCTION__;
    delete ft_thread_;
    ft_thread_ = 0;
}

//-----------------------------------------------------------------------------
void MainWindow::readerThreadFinished ()
{
    qDebug () << metaObject()->className() << "." << __FUNCTION__;
    delete reader_thread_;
    reader_thread_ = 0;

    try
    {
        if (client_->receiving())
            client_->stopReceiving();
        client_->disconnect ();
        delete client_;
    }
    catch (std::ios_base::failure& exc)
    {
        qDebug () << "Exception while disconnecting:" << exc.what();
    }
    client_ = 0;

}

//-------------------------------------------------------------------------------------------------
namespace MainWindowHelper
{
    //---------------------------------------------------------------------------------------------
    void monitorObjectLife (ApplicationMonitorDockWidget* monitor, QObject* object)
    {
        if (monitor)
            monitor->addObjectLifeMonitor (object);
    }
}


} } // namespace
