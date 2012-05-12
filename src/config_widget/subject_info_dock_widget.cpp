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
void SubjectInfoDockWidget::setSubjectInfo (tia::SubjectInfo const&  subject_info)
{

    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "id", subject_info.id().c_str());
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "first name", subject_info.firstName().c_str());
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "sur name", subject_info.surname().c_str());
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "brithday", subject_info.birthday().c_str());
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "medication", subject_info.medication().c_str());
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "handedness", subject_info.handedness() == tia::SubjectInfo::RightHanded ? "right-handed" : "left-handed");
    SubjectInfoDockWidgetHelper::addKeyAndValueToTable (ui->table, "sex", subject_info.sex() == tia::SubjectInfo::Male ? "male" : "female");

    //TODO: extract the subjects shortinfomap!
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
