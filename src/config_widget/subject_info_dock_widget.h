#ifndef SUBJECT_INFO_DOCKWIDGET_H
#define SUBJECT_INFO_DOCKWIDGET_H

#include "data_collector/qt_tia_client/tia_metainfo.h"

#include "tia/ss_meta_info.h"

#include <QDockWidget>

namespace Ui {
    class SubjectInfoDockWidget;
}

namespace TiAScope {

class SubjectInfoDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SubjectInfoDockWidget(QWidget *parent = 0);
    ~SubjectInfoDockWidget();

    void setSubjectInfo (TiAQtImplementation::SubjectInfoMap const& subject_info);

    void setSubjectInfo (tia::SubjectInfo const& subject_info);

public Q_SLOTS:
    void clear ();

private:
    Ui::SubjectInfoDockWidget *ui;
};

} // TiAScope

#endif // SUBJECT_INFO_DOCKWIDGET_H
