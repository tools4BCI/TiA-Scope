#ifndef TIA_EXCEPTION_H
#define TIA_EXCEPTION_H

#include <QString>
#include <stdexcept>

namespace TiAQtImplementation
{

class TiAException : public std::runtime_error
{
public:
    TiAException (QString const& what) : std::runtime_error (what.toStdString()) {}
    virtual ~TiAException () throw () {}
};

}

#endif // TIA_EXCEPTION_H
