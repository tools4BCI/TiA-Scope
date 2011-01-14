#include "subject_info_dock_widget.h"
#include "ui_subject_info_dock_widget.h"

#include <QTableWidgetItem>

namespace TiAScope {

namespace SubjectInfoDockWidgetHelper
{
    void addKeyAndValueToTable (QTableWidget* table, QString const& key, QString const& value);
}


SubjectInfoDockWidget::SubjectInfoDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui (new Ui::SubjectInfoDockWidget)
{
    ui->setupUi(this);
}

SubjectInfoDockWidget::~SubjectInfoDockWidget()
{
    delete ui;
}

//-----------------------------------------------------------------------------
void SubjectInfoDockWidget::setSubjectInfo (tobiss::SubjectInfo const& subject_info)
{
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "Id", subject_info.id().c_str());
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "First Name", subject_info.firstName().c_str());
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "Last Name", subject_info.surname().c_str());
}

//-----------------------------------------------------------------------------
void SubjectInfoDockWidget::clear ()
{
    ui->table->setRowCount (0);
}


//-----------------------------------------------------------------------------
namespace SubjectInfoDockWidgetHelper
{
    //-------------------------------------------------------------------------
    void addKeyAndValueToTable (QTableWidget* table, QString const& key, QString const& value)
    {
        QTableWidgetItem* key_item = new QTableWidgetItem ();
        QTableWidgetItem* value_item = new QTableWidgetItem ();

        key_item->setText (key);
        value_item->setText (value);

        table->setRowCount (table->rowCount() + 1);

        table->setItem (table->rowCount() - 1, 0, key_item);
        table->setItem (table->rowCount() - 1, 1, value_item);
    }
}


}
