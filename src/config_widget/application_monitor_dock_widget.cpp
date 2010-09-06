#include "application_monitor_dock_widget.h"
#include "ui_application_monitor_dock_widget.h"

#include <QDebug>

namespace tobiss { namespace scope {


//namespace ApplicationMonitorDockWidgetHelper
//{
//    class ObjectLifeConnector : public QObject
//    {
//        Q_OBJECT
//    public:
//        ObjectLifeConnector (QObject* parent) : QObject (parent) {}
//    Q_SIGNALS:
////        void objectDeleted ()
//    };
//}

//-------------------------------------------------------------------------------------------------
QList<ApplicationMonitorDockWidget*> ApplicationMonitorDockWidget::instances_;
QMutex ApplicationMonitorDockWidget::mutex_;

//-------------------------------------------------------------------------------------------------
ApplicationMonitorDockWidget::ApplicationMonitorDockWidget (QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ApplicationMonitorDockWidget)
{
    ui->setupUi (this);
    instances_.push_back (this);
}

//-------------------------------------------------------------------------------------------------
ApplicationMonitorDockWidget::~ApplicationMonitorDockWidget ()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void ApplicationMonitorDockWidget::addObjectLifeMonitor (QObject* object)
{
    ui->objectsList->addItem (object->objectName ());
    connect (object, SIGNAL(destroyed(QObject*)), SLOT(objectDestroyed(QObject*)));
}

//-------------------------------------------------------------------------------------------------
void ApplicationMonitorDockWidget::objectDestroyed (QObject* object)
{
    QList<QListWidgetItem*> items = ui->objectsList->findItems (object->objectName(), Qt::MatchExactly);
    Q_FOREACH (QListWidgetItem* item, items)
    {
        qDebug () << "remove item:"<< item->text();
        ui->objectsList->takeItem (ui->objectsList->row (item));
        delete item;
    }
}

//-------------------------------------------------------------------------------------------------
void ApplicationMonitorDockWidget::debugMessaging (QtMsgType type, const char* message)
{
    mutex_.lock();
    Q_FOREACH (ApplicationMonitorDockWidget* monitor, instances_)
    {
        monitor->ui->logTextEdit->appendPlainText (message);
    }
    mutex_.unlock();
}



} } // namespace
