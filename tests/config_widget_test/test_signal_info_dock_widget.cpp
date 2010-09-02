#include <QtTest/QtTest>

class TestSignalInfoDockWidget: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void toUpper();
};

void TestSignalInfoDockWidget::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(TestSignalInfoDockWidget)

#include "test_signal_info_dock_widget.moc"


