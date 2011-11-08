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
#include "config_widget/filter_dockwidget.h"
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

#include <algorithm>
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
    ft_thread_ (0),
    splitter_ (0),
    fft_view_ (0)
{
    ui->setupUi(this);
    initWelcomeScreen ();
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

//-----------------------------------------------------------------------------
void MainWindow::on_actionAutoScaling_toggled (bool checked)
{
    if (checked)
        helpers::animateProperty (signal_view_settings_.data(), "basicYScaling", signal_view_settings_->getBasicYScaling(), 1);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::on_actionReceiveData_toggled (bool checked)
{
    qDebug () << __FUNCTION__ << checked;
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
void MainWindow::setFTVisible (bool fts)
{
    if (fts)
        fft_view_->setVisible (true);
    else
        fft_view_->setVisible (false);
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
    ui->actionAutoScaling->setChecked (false);
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
    delete splitter_;
    splitter_ = 0;
    fft_view_ = 0;
}

//-----------------------------------------------------------------------------
void MainWindow::initDataViewScreen ()
{
    splitter_ = new QSplitter (Qt::Vertical, this);
    SignalGraphicsView* signal_view = new SignalGraphicsView (splitter_);
    SignalGraphicsScene* signals_graphics_scene = new SignalGraphicsScene (splitter_);
    signals_graphics_scene->connect (signal_view, SIGNAL(widthChanged(int)), SLOT(setSceneRectWidth(int)));

    if (!fft_view_)
        fft_view_ = new SignalGraphicsView (splitter_);

    fft_view_->hide();
    SignalGraphicsScene* fft_graphics_scene = new SignalGraphicsScene (splitter_);
    fft_graphics_scene->connect (fft_view_, SIGNAL(widthChanged(int)), SLOT(setSceneRectWidth(int)));

    splitter_->addWidget (signal_view);
    splitter_->addWidget (fft_view_);
    setCentralWidget (splitter_);
    ui->mainToolBar->show();
    ui->actionDisconnect->setEnabled (true);

    // init views
    TiAQtImplementation::TiAMetaInfo meta_info = qt_client_->getMetaInfo();
    signal_view_settings_ = QSharedPointer<SignalViewSettings> (new SignalViewSettings);
    signal_view_settings_->connect (ui->actionAutoScaling, SIGNAL(toggled(bool)), SLOT(setAutoScalingEnabled(bool)));
    ui->actionAutoScaling->setChecked (false);

    QSharedPointer<FTViewSettings> ft_view_settings (new FTViewSettings (meta_info));
    QSharedPointer<DataBuffer> data_buffer (new DataBuffer (qt_client_->getMetaInfo(), 30));

    ft_thread_ =  new FourierTransformThread (data_buffer, this);

    Q_FOREACH (TiAQtImplementation::SignalTypeFlag signal_type, meta_info.getSignalTypes())
    {
        if (!TiAQtImplementation::isAperiodic (signal_type))
        {
            SignalGraphicsObject* signal_object = new SignalGraphicsObject (signal_type, meta_info, data_buffer, signal_view_settings_);
            signal_object->setWidth (signal_view->width());
            signal_object->connect (signal_view, SIGNAL(widthChanged(int)), SLOT(setWidth(int)));
            signals_graphics_scene->addSignalGraphicsObject (signal_type, signal_object);

            SignalGraphicsObject* ft_signal_object = new SignalGraphicsObject (signal_type, meta_info, data_buffer, signal_view_settings_, ft_view_settings, ft_thread_);
            ft_signal_object->setWidth (fft_view_->width());
            ft_signal_object->connect (fft_view_, SIGNAL(widthChanged(int)), SLOT(setWidth(int)));
            fft_graphics_scene->addSignalGraphicsObject (signal_type, ft_signal_object);
        }
    }
    signal_view->setScene (signals_graphics_scene);
    signal_view->centerOn (0, 0);
    signals_graphics_scene->startTimer (40);
    fft_view_->setScene (fft_graphics_scene);

    reader_thread_ = new ReaderThread (data_buffer, qt_client_, false, this);

    // init signal info widget
    SignalInfoDockWidget* signal_info_widget = new SignalInfoDockWidget (signal_view_settings_, splitter_);
    dock_widgets_.append (signal_info_widget);
    addDockWidget (Qt::LeftDockWidgetArea, signal_info_widget);
    signal_info_widget->setSignalInfo (meta_info);
    signal_view_settings_->connect (signal_info_widget, SIGNAL(channelVisibilityChanged(SignalTypeFlag,ChannelID,bool)), SLOT(setChannelVisibility(SignalTypeFlag,ChannelID,bool)));
    signal_info_widget->setVisible (ui->actionViewSettings->isChecked());
    signal_info_widget->connect (ui->actionViewSettings, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));

    // init subject info widget
    SubjectInfoDockWidget* subject_info_widget = new SubjectInfoDockWidget (splitter_);
    dock_widgets_.append (subject_info_widget);
    addDockWidget (Qt::LeftDockWidgetArea, subject_info_widget);
    subject_info_widget->setSubjectInfo (meta_info.getSubjectInfo());
    subject_info_widget->setVisible (ui->actionSubjectInfo->isChecked());
    subject_info_widget->connect (ui->actionSubjectInfo, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));

    // init filters dock widget
    QList<double> samplingrates = meta_info.getSamplingRates().values();
    FilterDockWidget* filter_widget = new FilterDockWidget (*(std::min_element (samplingrates.constBegin(), samplingrates.constEnd())), splitter_);
    dock_widgets_.append (filter_widget);
    addDockWidget (Qt::LeftDockWidgetArea, filter_widget);
    filter_widget->hide ();
    filter_widget->setSignalInfo (meta_info);
    filter_widget->connect (ui->actionFilter, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
    connect (filter_widget, SIGNAL(anyFTEnabled(bool)), SLOT(setFTVisible(bool)));
    ft_thread_->connect (filter_widget, SIGNAL(signalChannelFTEnabledChanged(SignalTypeFlag,int,bool)), SLOT(enableFT(SignalTypeFlag,int,bool)), Qt::QueuedConnection);

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
