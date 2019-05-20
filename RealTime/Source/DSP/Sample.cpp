#include "Sample.h"

Sample Samples::Lerp(Sample a, Sample b, float f)
{
   return a + f * (b - a);
}

Sample Samples::Sinterp(Sample a, Sample b, float f)
{
   return a * cos(M_PI * f) + b * sin(M_PI * f);
}

Sample Samples::VertexFrom(Sample ax, Sample a, Sample b, Sample c)
{
    float R = -0.5;
    float bx = ax + 1;
    float cx = ax + 2;
    float A = (cx * (b - a) + bx * (a - c) + ax * (c - b)) * R;
    float B = (cx * cx * (a - b) + bx * bx * (c - a) + ax * ax * (b - c)) * R;
    
    return -B / (2*A);
}

Sample Samples::PeakFrom(Sample ax, Sample a, Sample b, Sample c)
{
    float R = -0.5f;
    float bx = ax + 1;
    float cx = ax + 2;
    float A = (cx * (b - a) + bx * (a - c) + ax * (c - b)) * R;
    float B = (cx * cx * (a - b) + bx * bx * (c - a) + ax * ax * (b - c)) * R;
    float C = (bx * cx * (bx - cx) * a + cx * ax * (cx - ax) * b + ax * bx * (ax - bx) * c) * R;
    
    return C - B*B / (4*A);
}

bool Samples::CrossedZero(Sample a, Sample b)
{
    return a * b < 0.0;
}

bool Samples::ChangedDirection(Sample a, Sample b, Sample c)
{
    return (b - a) * (c - b) < 0.0;
}

bool Samples::ConcaveUp(Sample a, Sample b ,Sample c)
{
     return (b - a) < (c - b);
}

bool Samples::ConcaveDown(Sample a, Sample b ,Sample c)
{
     return(b - a) > (c - b);
}
;
