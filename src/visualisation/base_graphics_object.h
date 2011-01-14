#ifndef BASE_GRAPHICS_OBJECT_H
#define BASE_GRAPHICS_OBJECT_H

#include <QGraphicsObject>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
class BaseGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY (double yScalingFactor READ yScalingFactor WRITE setYScalingFactor)
    Q_PROPERTY (int width READ width WRITE setWidth)
    Q_PROPERTY (int height READ height WRITE setHeight)
public:
    BaseGraphicsObject (QGraphicsItem* parent) : QGraphicsObject (parent), y_scaling_factor_ (1)
    {
    }

    double yScalingFactor () const {return y_scaling_factor_;}

    int width () const {return width_;}

    int height () const {return height_;}

    static int defaultHeight () {return 200;}

Q_SIGNALS:
    void bottomYChanged (int bottom_y);

public Q_SLOTS:
    void setYPos (int y_pos);
    void setYScalingFactor (double factor) {y_scaling_factor_ = factor;}
    void setWidth (int width) {width_ = width;}
    void setHeight (int height);

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
    int height_;
};

} // TiAScope

#endif // BASE_GRAPHICS_OBJECT_H
