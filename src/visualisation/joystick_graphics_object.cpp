#include "joystick_graphics_object.h"

#include <QPainter>
#include <QGtkStyle>

namespace tobiss { namespace scope {


//-------------------------------------------------------------------------------------------------
JoystickGraphicsObject::JoystickGraphicsObject (QGraphicsItem *parent) :
    AperiodicDataGraphicsObject (parent),
    layout_ (new QGraphicsLinearLayout (Qt::Horizontal, this))

{
    setLayout (layout_);
}

//-------------------------------------------------------------------------------------------------
void JoystickGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    int X_SPACING = 40;
    int x_pos = 20;

    Q_FOREACH (DeviceID device, data_.keys())
    {
        if (dials_.contains (device) == false)
        {
            dials_.insert (device, QVector<QDial*> (0));
            proxies_.insert (device, QVector<QGraphicsProxyWidget*> (0));
            for (int i = 0; i < data_[device].size(); i++)
            {
                QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget ();
                QDial* dial = new QDial ();
                dial->resize (X_SPACING * 2, X_SPACING * 2);
                dial->setMinimum (0);
                dial->setMaximum (0);
                dial->setValue (0);
                dial->setStyle (new QGtkStyle);
                //dial->setStyleSheet ("QDial {background: white; border: 1px solid black; color: black}");;
                proxy->setWidget (dial);
                dials_[device].append (dial);
                proxies_[device].append (proxy);
                layout_->addItem (proxy);
            }
        }
        painter->setPen (Qt::black);
        painter->setBrush (Qt::NoBrush);
        painter->drawText (x_pos, 50, QString ("Device \"").append(QString::number (device)).append("\":"));
        painter->drawRect (x_pos - (X_SPACING / 2), 40, x_pos + X_SPACING * data_[device].size(), 150);


        double value = 0;
        QDial* dial = 0;
        for (int i = 0; i < data_[device].size(); i++)
        {
            //proxies_[device][i]->setPos (x_pos, 10);
            dial = dials_[device][i];
            value = data_[device][i];
            if (dial->minimum() > value)
                dial->setMinimum (value);
            if (dial->maximum() < value)
                dial->setMaximum (value);
            dial->setValue (value);
            x_pos += X_SPACING;
        }
        x_pos += X_SPACING;
    }

}


} } // namespace

