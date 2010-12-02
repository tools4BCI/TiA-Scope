#ifndef SIGNAL_GRAPHICS_VIEW_H
#define SIGNAL_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

namespace tobiss { namespace scope {

class SignalGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SignalGraphicsView(QWidget *parent = 0);

Q_SIGNALS:
    void widthChanged (int width);

public Q_SLOTS:

private:
    void resizeEvent (QResizeEvent* event) {Q_EMIT widthChanged (event->size().width());}

};

} } // namespace

#endif // SIGNAL_GRAPHICS_VIEW_H
