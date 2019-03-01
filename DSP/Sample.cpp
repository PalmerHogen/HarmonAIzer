#include "Sample.h"

Sample Samples::Lerp(Sample a, Sample b, double f)
{
   return a + f * (b - a);
}

Sample Samples::Sinterp(Sample a, Sample b, double f)
{
   return a * cos(M_PI * f) + b * sin(M_PI * f);
}

Sample Samples::VertexFrom(double ax, Sample a, Sample b, Sample c)
{
    double R = -0.5;
    double bx = ax + 1;
    double cx = ax + 2;
    double A = (cx * (b - a) + bx * (a - c) + ax * (c - b)) * R;
    double B = (cx * cx * (a - b) + bx * bx * (c - a) + ax * ax * (b - c)) * R;
    
    return -B / (2*A);
}

Sample Samples::PeakFrom(double ax, Sample a, Sample b, Sample c)
{
    double R = -0.5;
    double bx = ax + 1;
    double cx = ax + 2;
    double A = (cx * (b - a) + bx * (a - c) + ax * (c - b)) * R;
    double B = (cx * cx * (a - b) + bx * bx * (c - a) + ax * ax * (b - c)) * R;
    double C = (bx * cx * (bx - cx) * a + cx * ax * (cx - ax) * b + ax * bx * (ax - bx) * c) * R;
    
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
