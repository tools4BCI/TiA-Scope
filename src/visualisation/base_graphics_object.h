#ifndef BASE_GRAPHICS_OBJECT_H
#define BASE_GRAPHICS_OBJECT_H

#include <QGraphicsObject>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
class BaseGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY (double yScalingFactor READ yScalingFactor WRITE setYScalingFactor)
    Q_PROPERTY (int width READ width WRITE setWidth)
public:
    BaseGraphicsObject (QGraphicsItem* parent) : QGraphicsObject (parent), y_scaling_factor_ (1)
    {
        //setFlags (flags() | QGraphicsItem::ItemIsMovable);
    }

    double yScalingFactor () const {return y_scaling_factor_;}

    int width () const {return width_;}

public Q_SLOTS:
    void setYScalingFactor (double factor) {y_scaling_factor_ = factor;}
    void setWidth (int width) {width_ = width;}

private:
    virtual void wheelEvent (QGraphicsSceneWheelEvent* event)
    {
        if (event->modifiers().testFlag (Qt::ControlModifier))
        {
            event->accept ();
            if (event->delta() > 0)
                y_scaling_factor_ *= 2;
            else if (event->delta() < 0)
                y_scaling_factor_ /= 2;
        }
        else
            event->ignore ();
    }

    double y_scaling_factor_;
    int width_;
};

} } // namespace

#endif // BASE_GRAPHICS_OBJECT_H
