#include "user_types.h"

#include "definitions/constants.h"

namespace tobiss { namespace scope {

namespace TypeConverter {
    Constants constants;

    //---------------------------------------------------------------------------------------------
    SignalTypeFlag stdStringToSignalTypeFlag (std::string const& type_string)
    {
        return constants.getSignalFlag (type_string);
    }
}

} } // namespace
