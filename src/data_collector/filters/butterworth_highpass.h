#ifndef BUTTERWORTH_HIGHPASS_H
#define BUTTERWORTH_HIGHPASS_H

#include "filter.h"
#include "SPUC/iir_2nd.h"

namespace TiAScope
{

class ButterworthHighpass : public Filter
{
    public:

        /**
         * The parameters of this filters comply with the matlab script 'highpass_filters.m'!
         */
        static const int NSPECS = 14;
        static const int ORDER = 2;
        static const double NORMFREQS[];
        static const double INGAIN[];
        static const double OUTGAIN[];
        static const double NUM[NSPECS][ORDER+1];
        static const double DEN[NSPECS][ORDER];

        ButterworthHighpass(int spec_pos);

        virtual ~ButterworthHighpass() {if (filter_) delete filter_;}

        double clock(double sample);

private:
        SPUC::iir_2nd<double> *filter_;
        int spec_pos_;
};

}
#endif // BUTTERWORTH_HIGHPASS_H
