#pragma once

#include <math.h>

typedef double Sample;

namespace Samples
{
    Sample Lerp(Sample a, Sample b, double f);
    
    Sample Sinterp(Sample a, Sample b, double f);
    
    Sample VertexFrom(double ax, Sample a, Sample b, Sample c);
    
    Sample PeakFrom(double ax, Sample a, Sample b, Sample c);
    
    bool CrossedZero(Sample a, Sample b);
    
    bool ChangedDirection(Sample a, Sample b, Sample c);
    
    bool ConcaveUp(Sample a, Sample b, Sample c);
    
    bool ConcaveDown(Sample a, Sample b, Sample c);
};
