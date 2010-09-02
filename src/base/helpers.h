#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>
#include <QByteArray>
#include <QSettings>
#include <QPropertyAnimation>

namespace tobiss { namespace scope {

namespace helpers
{


//-------------------------------------------------------------------------------------------------
void animateProperty (QObject* target, QByteArray const& property_name,
                      QVariant const& start_value, QVariant const& end_value,
                      QObject* call_back_object = 0, char const* call_back_slot = 0);


} } } // namespace


#endif // HELPERS_H
