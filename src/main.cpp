#include <QtGui/QApplication>
#include "main_window.h"

#include <QVector>
#include <QSharedPointer>
#include <QMetaType>

using namespace tobiss::scope;

int main(int argc, char *argv[])
{

    qRegisterMetaType<QVector<double> > ("QVector<double>");
    QApplication a(argc, argv);
    QApplication::setOrganizationName ("BCI Lab");
    QApplication::setOrganizationDomain("http://bci.tugraz.at");
    QApplication::setApplicationName ("TOBI Remote Scope");

    MainWindow w;
    w.show();

    return a.exec();
}
