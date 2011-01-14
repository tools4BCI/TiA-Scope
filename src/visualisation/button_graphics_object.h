#ifndef BUTTON_GRAPHICS_OBJECT_H
#define BUTTON_GRAPHICS_OBJECT_H

#include "base/data_buffer.h"
#include "aperiodic_data_graphics_object.h"

#include <QSharedPointer>
#include <QGraphicsWidget>

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
class ButtonGraphicsObject : public AperiodicDataGraphicsObject
{
    Q_OBJECT
public:
    explicit ButtonGraphicsObject (QGraphicsItem *parent) : AperiodicDataGraphicsObject (parent) {}

private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};

} // TiAScope

#endif // BUTTON_GRAPHICS_OBJECT_H
