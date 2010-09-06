#ifndef JOYSTICK_GRAPHICS_OBJECT_H
#define JOYSTICK_GRAPHICS_OBJECT_H


#include "base/data_buffer.h"
#include "aperiodic_data_graphics_object.h"

#include <QDial>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
class JoystickGraphicsObject : public AperiodicDataGraphicsObject
{
    Q_OBJECT
public:
    explicit JoystickGraphicsObject (QGraphicsItem *parent);

private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QMap<DeviceID, QVector<QDial*> > dials_;
    QMap<DeviceID, QVector<QGraphicsProxyWidget*> > proxies_;
    QGraphicsLinearLayout* layout_;
};

} } // namespace

#endif // JOYSTICK_GRAPHICS_OBJECT_H
