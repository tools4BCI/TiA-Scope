#include "joystick_graphics_object.h"

#include <QPainter>
#include <QGtkStyle>

namespace TiAScope {


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
                //dial->setStyle (new QGtkStyle);
                proxy->setWidget (dial);
                dials_[device].append (dial);
                proxies_[device].append (proxy);
                layout_->addItem (proxy);
            }
        }

        double value = 0;
        QDial* dial = 0;
        for (int i = 0; i < data_[device].size(); i++)
        {
            dial = dials_[device][i];
            value = data_[device][i];
            if (dial->minimum() > value)
                dial->setMinimum (value);
            if (dial->maximum() < value)
                dial->setMaximum (value);
            dial->setValue (value);
        }
    }

}


} // TiAScope

