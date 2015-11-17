#include <QtGui/QApplication>
#include "main_window.h"
#include "base/user_types.h"

#include <QVector>
#include <QSharedPointer>
#include <QMetaType>
#include <QTextBlock>
#include <QTextCursor>
#include <QSettings>

using namespace TiAScope;

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

    QSettings settings(QString("tiascope.ini"),QSettings::IniFormat);
    bool maximized = settings.value ("mainwindow/maximized", false).toBool();
    int height = settings.value ("mainwindow/height", 600).toInt();
    int width = settings.value ("mainwindow/width", 1300).toInt();

    MainWindow w;

    w.resize(width, height);

    if (maximized)
        w.showMaximized();
    else
        w.show();



    return a.exec();
}
