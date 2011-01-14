#include "user_types.h"

#include "tia/constants.h"

namespace TiAScope {

namespace TypeConverter {
    tobiss::Constants constants;

    //---------------------------------------------------------------------------------------------
    SignalTypeFlag stdStringToSignalTypeFlag (std::string const& type_string)
    {
        return constants.getSignalFlag (type_string);
    }
}

} // TiAScope
