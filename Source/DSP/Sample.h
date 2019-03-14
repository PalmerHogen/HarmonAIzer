#pragma once

#include <math.h>

typedef float Sample;

namespace Samples
{
    Sample Lerp(Sample a, Sample b, float f);
    
    Sample Sinterp(Sample a, Sample b, float f);
    
    Sample VertexFrom(Sample ax, Sample a, Sample b, Sample c);
    
    Sample PeakFrom(Sample ax, Sample a, Sample b, Sample c);
    
    bool CrossedZero(Sample a, Sample b);
    
    bool ChangedDirection(Sample a, Sample b, Sample c);
    
    bool ConcaveUp(Sample a, Sample b, Sample c);
    
    bool ConcaveDown(Sample a, Sample b, Sample c);
};
