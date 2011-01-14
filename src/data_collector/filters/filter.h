#ifndef FILTER_H
#define FILTER_H

namespace TiAScope
{

class Filter
{
public:
    virtual ~Filter () {}

    virtual double clock (double sample) = 0;
};

}

#endif // FILTER_H
