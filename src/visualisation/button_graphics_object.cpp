#include "button_graphics_object.h"

#include "definitions/defines.h"

#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QStyle>
#include <QWidget>
#include <QToolButton>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
ButtonGraphicsObject::ButtonGraphicsObject (QSharedPointer<DataBuffer const> data_buffer, QGraphicsItem *parent) :
    QGraphicsWidget (parent),
    data_buffer_ (data_buffer)
{
    //QToolButton* bla = new QToolButton (this);
    //bla->setIcon (style()->standardIcon (QStyle::SP_DialogCloseButton));
}

//-------------------------------------------------------------------------------------------------
void ButtonGraphicsObject::updateView ()
{
    QList<DeviceID> devices = data_buffer_->getAperiodicDeviceIDs (SIG_BUTTON);

    Q_FOREACH (DeviceID device, devices)
    {
        if (device_button_pressed_.contains (device) == false)
            device_button_pressed_.insert (device, QVector<bool>());
        QVector<bool>& pressed_vector = device_button_pressed_[device];
        QList<double> values = data_buffer_->getAperiodicValues (SIG_BUTTON, device);
        pressed_vector.resize (values.size());
        for (int index = 0; index < values.size(); index++)
        {
            pressed_vector[index] = values[index] > 0;
        }
    }

    update ();
}


//-------------------------------------------------------------------------------------------------
void ButtonGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    int X_SPACING = 25;
    int x_pos = 20;

    Q_FOREACH (DeviceID device, device_button_pressed_.keys())
    {
        painter->drawText (x_pos, 50, QString ("Device \"").append(QString::number (device)).append("\":"));
        painter->drawRect (x_pos - (X_SPACING / 2), 40, x_pos + X_SPACING * device_button_pressed_[device].size(), 150);
        Q_FOREACH (bool button_pressed, device_button_pressed_[device])
        {
            if (button_pressed)
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
