#include "base_graphics_object.h"

namespace TiAScope {


//-----------------------------------------------------------------------------
void BaseGraphicsObject::setHeight (int height)
{
    height_ = height;
    Q_EMIT bottomYChanged (y() + height_);
}

//-----------------------------------------------------------------------------
void BaseGraphicsObject::setYPos (int y_pos)
{
    setY (y_pos);
    Q_EMIT bottomYChanged (y() + height_);
}



} // TiAScope
