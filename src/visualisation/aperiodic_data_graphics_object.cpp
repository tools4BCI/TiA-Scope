#include "aperiodic_data_graphics_object.h"
#include "button_graphics_object.h"
#include "joystick_graphics_object.h"

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
AperiodicDataGraphicsObject* AperiodicDataGraphicsObject::createAperiodicDataGraphicsObject (SignalTypeFlag signal, QSharedPointer<DataBuffer const> data_buffer, QGraphicsItem *parent)
{
    AperiodicDataGraphicsObject* object = 0;
    switch (signal)
    {
    case TiAQtImplementation::SIGNAL_TYPE_Buttons:
        object = new ButtonGraphicsObject (parent);
        break;
    case TiAQtImplementation::SIGNAL_TYPE_Joystick:
        object = new JoystickGraphicsObject (parent);
        break;
    }
    if (object)
    {
        object->signal_type_ = signal;
        object->data_buffer_ = data_buffer;
    }
    return object;
}

//-------------------------------------------------------------------------------------------------
void AperiodicDataGraphicsObject::updateView ()
{
    QList<DeviceID> devices = data_buffer_->getAperiodicDeviceIDs (signal_type_);

    Q_FOREACH (DeviceID device, devices)
    {
        if (data_.contains (device) == false)
            data_.insert (device, QVector<double>());
        QVector<double>& data_vector = data_[device];
        QList<double> values = data_buffer_->getAperiodicValues (signal_type_, device);
        data_vector.resize (values.size());
        for (int index = 0; index < values.size(); index++)
        {
            data_vector[index] = values[index];
        }
    }

    update ();
}


} // TiAScope
