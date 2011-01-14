#include "butterworth_filter.h"

namespace TiAScope
{

ButterworthFilter::ButterworthFilter (double samplerate, double cutoff_frequence)
    : filter_ (new SPUC::butterworth<double> (cutoff_frequence / samplerate, 2, 3))
{

}

double ButterworthFilter::clock (double sample)
{
    return filter_->clock (sample);
}

}
