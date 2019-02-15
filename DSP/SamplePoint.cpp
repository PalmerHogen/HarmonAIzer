#include "SamplePoint.h"
#include <math.h>

SamplePoint::SamplePoint(){}

SamplePoint::SamplePoint(double* signal, int sample)
{
    X = sample;
    Y = signal[sample];
}

SamplePoint::SamplePoint(double sample, double value)
{
    X = sample;
    Y = value;
}

SamplePoint::~SamplePoint(){}

double SamplePoint::Sample()
{
    return X;
}

double SamplePoint::Value()
{
    return Y;
}

double SamplePoint::SlopeOf(SamplePoint a, SamplePoint b)
{
    if(a.IsAdjacentTo(b))
        return SlopeOfAdjacent(a, b);
    return (b.Y - a.Y) / (b.X - a.X);
}

double SamplePoint::SlopeOfAdjacent(SamplePoint a, SamplePoint b)
{
    return b.Y - a.Y;
}

SamplePoint SamplePoint::Lerp(SamplePoint a, SamplePoint b, double f)
{
    return SamplePoint(a.X + (b.X - a.X * f), a.Y + (b.Y - a.Y * f));
}

SamplePoint SamplePoint::Sinterp(SamplePoint a, SamplePoint b, double f)
{
    return SamplePoint(a.X * cos(M_PI * f) + b.X * sin(M_PI * f), a.Y * cos(M_PI * f) + b.Y * sin(M_PI * f));
}

SamplePoint SamplePoint::VertexOf(SamplePoint a, SamplePoint b, SamplePoint c)
{
    double R = 1.0 / ((a.X - b.X) * (a.X - c.X) * (b.X - c.X));
    double A = (c.X * (b.Y - c.Y) + b.X * (a.Y - c.Y) + a.X * (c.Y - b.Y)) * R;
    double B = (c.X * c.X * (a.Y - b.Y) + b.X * b.X * (c.Y - a.Y) + a.X * a.X * (b.Y - c.Y)) * R;
    double C = (b.X * c.X * (b.X - c.X) * a.Y + c.X * a.X * (c.X - a.X) * b.Y + a.X * b.X * (a.X - b.X) * c.Y) * R;
    
    return SamplePoint(-B / (2*A), C - B*B / (4*A));
}

SamplePoint SamplePoint::VertexOfAdjacent(SamplePoint a, SamplePoint b, SamplePoint c)
{
    double R = -0.5;
    double A = (c.X * (b.Y - c.Y) + b.X * (a.Y - c.Y) + a.X * (c.Y - b.Y)) * R;
    double B = (c.X * c.X * (a.Y - b.Y) + b.X * b.X * (c.Y - a.Y) + a.X * a.X * (b.Y - c.Y)) * R;
    double C = (b.X * c.X * (b.X - c.X) * a.Y + c.X * a.X * (c.X - a.X) * b.Y + a.X * b.X * (a.X - b.X) * c.Y) * R;
    
    return SamplePoint(-B / (2*A), C - B*B / (4*A));
}

bool SamplePoint::CrossedZero(SamplePoint a, SamplePoint b)
{
    return a.Y * b.Y < 0.0;
}

bool SamplePoint::ChangedDirection(SamplePoint a, SamplePoint b, SamplePoint c)
{
    return SlopeOf(a, b) * SlopeOf(b, c) < 0.0;
}

bool SamplePoint::AdjacentsChangedDirection(SamplePoint a, SamplePoint b, SamplePoint c)
{
    return SlopeOfAdjacent(a, b) * SlopeOfAdjacent(b, c) < 0.0;
}

bool SamplePoint::ConcaveUp(SamplePoint a, SamplePoint b, SamplePoint c)
{
    return SlopeOfAdjacent(a, b) < SlopeOfAdjacent(b, c);
}

bool SamplePoint::ConcaveDown(SamplePoint a, SamplePoint b, SamplePoint c)
{
    return SlopeOfAdjacent(a, b) > SlopeOfAdjacent(b, c);
}

bool SamplePoint::IsAdjacentTo(SamplePoint other)
{
    return other.X - X == 1.0;
}
