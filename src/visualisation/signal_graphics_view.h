#ifndef SIGNAL_GRAPHICS_VIEW_H
#define SIGNAL_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

namespace TiAScope {

class SignalGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SignalGraphicsView(QWidget *parent = 0) : QGraphicsView(parent) {}

Q_SIGNALS:
    void widthChanged (int width);

public Q_SLOTS:

private:
    void resizeEvent (QResizeEvent* event) {Q_EMIT widthChanged (event->size().width());}

};

} // TiAScope

#endif // SIGNAL_GRAPHICS_VIEW_H
