#ifndef USER_TYPES_H
#define USER_TYPES_H

#include <boost/cstdint.hpp>

#include <string>

namespace TiAScope {

typedef boost::uint32_t SignalTypeFlag;
typedef boost::uint32_t DeviceID;
typedef int ChannelID;

namespace TypeConverter {
    SignalTypeFlag stdStringToSignalTypeFlag (std::string const& type_string);
}

} // TiAScope

#endif // USER_TYPES_H
