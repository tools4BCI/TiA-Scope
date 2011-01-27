#include "main_window.h"
#include "ui_main_window.h"

#include "base/data_buffer.h"
#include "base/ft_view_settings.h"
#include "base/helpers.h"

#include "visualisation/signal_graphics_object.h"
#include "visualisation/signal_graphics_scene.h"
#include "visualisation/signal_graphics_view.h"

#include "config_widget/signal_info_dock_widget.h"
#include "config_widget/subject_info_dock_widget.h"
#include "config_widget/view_settings_dock_widget.h"
#include "config_widget/application_monitor_dock_widget.h"

#include "connection_widget/connect_dialog.h"
#include "connection_widget/welcome_widget.h"

#include "data_collector/reader_thread.h"
#include "data_collector/fourier_transform_thread.h"
#include "data_collector/qt_tia_client/impl/tia_client_version02.h"

#include <QCoreApplication>
#include <QSharedPointer>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QDebug>

#include <iostream>

namespace TiAScope {

namespace MainWindowHelper
{
    inline void monitorObjectLife (ApplicationMonitorDockWidget* monitor, QObject* object);
}

//-----------------------------------------------------------------------------
MainWindow::MainWindow (QWidget *parent) :
    QMainWindow (parent),
    ui (new Ui::MainWindow),
    reader_thread_ (0),
    ft_thread_ (0)
{
    ui->setupUi(this);
    initWelcomeScreen ();

    //view_settings_widget_ = new ViewSettingsDockWidget (this);
    // addDockWidget (Qt::LeftDockWidgetArea, view_settings_widget_);
    //view_settings_widget_->hide();

    //signal_info_widget_ = new SignalInfoDockWidget (this);
    // addDockWidget (Qt::RightDockWidgetArea, signal_info_widget_);
    //signal_info_widget_->hide();

    //subject_info_widget_ = new SubjectInfoDockWidget (this);
    // addDockWidget (Qt::RightDockWidgetArea, subject_info_widget_);
    //subject_info_widget_->hide();

//    if (QCoreApplication::arguments().contains ("--test") || QCoreApplication::arguments().contains("-t"))
//    {
//        monitor_widget_ = new ApplicationMonitorDockWidget (this);
//        qInstallMsgHandler (ApplicationMonitorDockWidget::debugMessaging);
//        addDockWidget (Qt::LeftDockWidgetArea, monitor_widget_);
//    }
//    view_->setInteractive (true);
}

//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
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
    initWelcomeScreen ();
}

//-----------------------------------------------------------------------------
void MainWindow::on_actionZoomIn_triggered ()
{
    helpers::animateProperty (signal_view_settings_.data(), "basicYScaling", signal_view_settings_->getBasicYScaling(), signal_view_settings_->getBasicYScaling() * 2);
}

//-----------------------------------------------------------------------------
void MainWindow::on_actionZoomOut_triggered ()
{
    helpers::animateProperty (signal_view_settings_.data(), "basicYScaling", signal_view_settings_->getBasicYScaling(), signal_view_settings_->getBasicYScaling() / 2);
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

//-------------------------------------------------------------------------------------------------
void MainWindow::startConnection (QSharedPointer<TiAQtImplementation::TiAQtClient> new_client)
{
    qt_client_ = new_client;

    initDataViewScreen ();
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
        if (qt_client_)
            qt_client_->disconnectFromServer();
        qt_client_.clear ();
    }
    catch (std::exception& exc)
    {
        qDebug () << "Exception while disconnecting:" << exc.what();
    }
}

//-----------------------------------------------------------------------------
void MainWindow::initWelcomeScreen ()
{
    ui->actionSubjectInfo->setChecked (false);
    ui->actionViewSettings->setChecked (false);
    Q_FOREACH (QDockWidget* dock_widget, dock_widgets_)
    {
        removeDockWidget (dock_widget);
        delete dock_widget;
    }
    dock_widgets_.clear ();
    signal_view_settings_.clear ();
    WelcomeWidget* welcome_widget = new WelcomeWidget (this);
    setCentralWidget (welcome_widget);
    welcome_widget->show();
    connect (welcome_widget, SIGNAL(connectionSelected(QSharedPointer<TiAQtImplementation::TiAQtClient>)), SLOT(startConnection(QSharedPointer<TiAQtImplementation::TiAQtClient>)));
    ui->mainToolBar->hide ();
}

//-----------------------------------------------------------------------------
void MainWindow::initDataViewScreen ()
{
    QSplitter* splitter = new QSplitter (Qt::Vertical, this);
    SignalGraphicsView* signal_view = new SignalGraphicsView (splitter);
    SignalGraphicsScene* signals_graphics_scene = new SignalGraphicsScene (splitter);
    signals_graphics_scene->connect (signal_view, SIGNAL(widthChanged(int)), SLOT(setSceneRectWidth(int)));

//    if (!fft_view_)
//        fft_view_ = new SignalGraphicsView (splitter);

//    fft_view_->hide();
    splitter->addWidget (signal_view);
    setCentralWidget (splitter);
    ui->mainToolBar->show();
    ui->actionDisconnect->setEnabled (true);

    // init views
    TiAQtImplementation::TiAMetaInfo meta_info = qt_client_->getMetaInfo();
    signal_view_settings_ = QSharedPointer<SignalViewSettings> (new SignalViewSettings);
    //QSharedPointer<FTViewSettings> ft_view_settings (new FTViewSettings (meta_info));
    //subject_info_widget_->setSubjectInfo (meta_info.getSubjectInfo ());
    //view_settings_widget_->setSignalViewSettings (signal_view_settings);
    //view_settings_widget_->setFTViewSettings (ft_view_settings);
    QSharedPointer<DataBuffer> data_buffer (new DataBuffer (qt_client_->getMetaInfo(), 30));
    //MainWindowHelper::monitorObjectLife (monitor_widget_, data_buffer.data());

    ft_thread_ =  new FourierTransformThread (data_buffer, this);
    //MainWindowHelper::monitorObjectLife (monitor_widget_, ft_thread_);

    //fft_graphics_scene_ = new SignalGraphicsScene (this);
    //fft_graphics_scene_->connect (fft_view_, SIGNAL(widthChanged(int)), SLOT(setSeneRectWidth(int)));

    //MainWindowHelper::monitorObjectLife (monitor_widget_, graphics_scene_);
    //ft_thread_->connect (signal_info_widget_, SIGNAL(signalChannelFTEnabledChanged(SignalTypeFlag,int,bool)), SLOT(enableFT(SignalTypeFlag,int,bool)), Qt::QueuedConnection);

    Q_FOREACH (TiAQtImplementation::SignalTypeFlag signal_type, meta_info.getSignalTypes())
    {
        SignalGraphicsObject* signal_object = new SignalGraphicsObject (signal_type, meta_info, data_buffer, signal_view_settings_);
        signal_object->setWidth (signal_view->width());
        signal_object->connect (signal_view, SIGNAL(widthChanged(int)), SLOT(setWidth(int)));
        signals_graphics_scene->addSignalGraphicsObject (signal_type, signal_object);

        //SignalGraphicsObject* ft_signal_object = new SignalGraphicsObject (signal_type, meta_info, data_buffer, signal_view_settings, ft_view_settings, ft_thread_);
        //ft_signal_object->setWidth (fft_view_->width());
        //ft_signal_object->connect (fft_view_, SIGNAL(widthChanged(int)), SLOT(setWidth(int)));
        //fft_graphics_scene_->addSignalGraphicsObject (signal_type, ft_signal_object);
    }
    signal_view->setScene (signals_graphics_scene);
    signal_view->centerOn (0, 0);
    signals_graphics_scene->startTimer (40);

    //fft_view_->setScene (fft_graphics_scene_);

    reader_thread_ = new ReaderThread (data_buffer, qt_client_, false, this);

    // init signal info widget
    SignalInfoDockWidget* signal_info_widget = new SignalInfoDockWidget (signal_view_settings_, this);
    dock_widgets_.append (signal_info_widget);
    addDockWidget (Qt::LeftDockWidgetArea, signal_info_widget);
    signal_info_widget->setSignalInfo (meta_info);
    signal_view_settings_->connect (signal_info_widget, SIGNAL(channelVisibilityChanged(SignalTypeFlag,ChannelID,bool)), SLOT(setChannelVisibility(SignalTypeFlag,ChannelID,bool)));
    signal_info_widget->setVisible (ui->actionViewSettings->isChecked());
    signal_info_widget->connect (ui->actionViewSettings, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));

    // init subject info widget
    SubjectInfoDockWidget* subject_info_widget = new SubjectInfoDockWidget (this);
    dock_widgets_.append (subject_info_widget);
    addDockWidget (Qt::LeftDockWidgetArea, subject_info_widget);
    subject_info_widget->setSubjectInfo (meta_info.getSubjectInfo());
    subject_info_widget->setVisible (ui->actionSubjectInfo->isChecked());
    subject_info_widget->connect (ui->actionSubjectInfo, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));

    ui->actionReceiveData->setEnabled (true);
    ui->actionReceiveData->setChecked (true);
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


} // TiAScope
