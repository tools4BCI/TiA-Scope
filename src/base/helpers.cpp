#include "helpers.h"

namespace TiAScope {

namespace helpers
{


//-------------------------------------------------------------------------------------------------
void animateProperty (QObject* target, QByteArray const& property_name,
                      QVariant const& start_value, QVariant const& end_value,
                      QObject* call_back_object, char const* call_back_slot)
{
    QSettings settings;
    bool animations_activated = settings.value ("gui_animations/activated", true).toBool();
    int animation_duration = settings.value ("gui_animations/duration", 200).toInt();

    if (animations_activated)
    {
        QPropertyAnimation* animation = new QPropertyAnimation (target,
                                                                    property_name);
        animation->setDuration (animation_duration);
        animation->setStartValue (start_value);
        animation->setEasingCurve (QEasingCurve::InOutCubic);
        animation->setEndValue (end_value);
        if (call_back_object && call_back_slot)
            call_back_object->connect (animation, SIGNAL(finished()), call_back_slot);
        animation->start (animation->DeleteWhenStopped);
    }
    else
    {
        target->setProperty (property_name, end_value);
        if (call_back_object && call_back_slot)
        {
            QMetaObject::invokeMethod (call_back_object, call_back_slot);
        }
    }
}


} } // TiAScope
