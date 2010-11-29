#ifndef SUBJECT_INFO_DOCKWIDGET_H
#define SUBJECT_INFO_DOCKWIDGET_H

#include "tia/ss_meta_info.h"

#include <QDockWidget>

namespace Ui {
    class SubjectInfoDockWidget;
}

namespace tobiss { namespace scope {

class SubjectInfoDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SubjectInfoDockWidget(QWidget *parent = 0);
    ~SubjectInfoDockWidget();

    void setSubjectInfo (SubjectInfo const& subject_info);

private:
    Ui::SubjectInfoDockWidget *ui;
};

} }

#endif // SUBJECT_INFO_DOCKWIDGET_H
