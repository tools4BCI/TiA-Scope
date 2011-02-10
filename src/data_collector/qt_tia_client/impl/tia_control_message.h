#ifndef TIA_CONTROL_MESSAGE_H
#define TIA_CONTROL_MESSAGE_H

#include <QString>

namespace TiAQtImplementation
{

struct TiAControlMessage
{
    QString version;
    QString command;
    QString parameter;
    QString content;
};

}

#endif // TIA_CONTROL_MESSAGE_H
