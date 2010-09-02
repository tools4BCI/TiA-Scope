#ifndef BUTTON_GRAPHICS_OBJECT_H
#define BUTTON_GRAPHICS_OBJECT_H

#include "base/data_buffer.h"

#include <QSharedPointer>
#include <QGraphicsWidget>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
class ButtonGraphicsObject : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit ButtonGraphicsObject (QSharedPointer<DataBuffer const> data_buffer, QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const {return QRectF(0, 0, 200, 400);}
public Q_SLOTS:
    void updateView ();

private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QMap<DeviceID, QVector<bool> > device_button_pressed_;
    QSharedPointer<DataBuffer const> data_buffer_;
};

} } // namespace

#endif // BUTTON_GRAPHICS_OBJECT_H
