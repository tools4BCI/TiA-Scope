#include <QtGui/QApplication>
#include "main_window.h"
#include "base/user_types.h"

#include <QVector>
#include <QSharedPointer>
#include <QMetaType>
#include <QTextBlock>
#include <QTextCursor>

using namespace tobiss::scope;

int main(int argc, char *argv[])
{
    qRegisterMetaType<QTextBlock> ("QTextBlock");
    qRegisterMetaType<QTextCursor> ("QTextCursor");
    qRegisterMetaType<QVector<double> > ("QVector<double>");
    qRegisterMetaType<SignalTypeFlag> ("SignalTypeFlag");
    QApplication a(argc, argv);
    QApplication::setOrganizationName ("BCI Lab");
    QApplication::setOrganizationDomain("http://bci.tugraz.at");
    QApplication::setApplicationName ("TOBI Remote Scope");

    MainWindow w;
    w.show();

    return a.exec();
}
