#include "button_graphics_object.h"

#include <QPainter>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
void ButtonGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    int X_SPACING = 25;
    int x_pos = 20;

    Q_FOREACH (DeviceID device, data_.keys())
    {
        painter->setPen (Qt::black);
        painter->setBrush (Qt::NoBrush);
        painter->drawText (x_pos, 50, QString ("Device \"").append(QString::number (device)).append("\":"));
        painter->drawRect (x_pos - (X_SPACING / 2), 40, x_pos + X_SPACING * data_[device].size(), 150);
        Q_FOREACH (double button_pressed, data_[device])
        {
            if (button_pressed > 0)
                painter->setPen (Qt::green);
            else
                painter->setPen (Qt::red);

            painter->setBrush (Qt::SolidPattern);
            QBrush brush = painter->brush();
            brush.setColor(painter->pen().color());
            painter->setBrush(brush);

            painter->drawEllipse (QPoint(x_pos, 100), 7, 7);
            x_pos += X_SPACING;
        }
        x_pos += X_SPACING;
    }

}


} } // namespace
