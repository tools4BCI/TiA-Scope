#ifndef BUTTERWORTH_FILTER_H
#define BUTTERWORTH_FILTER_H

#include "filter.h"

#include "SPUC/butterworth.h"

namespace TiAScope
{

class ButterworthFilter : public Filter
{
public:
    ButterworthFilter (double samplerate, double cutoff_frequence);

    virtual ~ButterworthFilter () {if (filter_) delete filter_;}

    double clock (double sample);

private:
    SPUC::butterworth<double>* filter_;
};

}

#endif // BUTTERWORTH_FILTER_H
