#include "butterworth_filter.h"

namespace TiAScope
{

ButterworthFilter::ButterworthFilter (double samplerate, double cutoff_frequence)
    : filter_ (0)
{
    if (samplerate > cutoff_frequence)
        filter_ = new SPUC::butterworth<double> (cutoff_frequence / samplerate, 4, 5);
}

double ButterworthFilter::clock (double sample)
{
    if (filter_)
        return filter_->clock (sample);
    else
        return sample;
}

}
