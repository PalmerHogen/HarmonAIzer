#pragma once

class SamplePoint
{
public:
    SamplePoint();
    SamplePoint(double* signal, int sample);
    SamplePoint(double sample, double value);
    ~SamplePoint();
    
    double Sample();
    double Value();
    
    static double SlopeOf(SamplePoint a, SamplePoint b);
    static double SlopeOfAdjacent(SamplePoint a, SamplePoint b);
    
    static SamplePoint Lerp(SamplePoint a, SamplePoint b, double f);
    static SamplePoint Sinterp(SamplePoint a, SamplePoint b, double f);
    static SamplePoint VertexOf(SamplePoint a, SamplePoint b, SamplePoint c);
    static SamplePoint VertexOfAdjacent(SamplePoint a, SamplePoint b, SamplePoint c);
    
    static bool CrossedZero(SamplePoint a, SamplePoint b);
    static bool ChangedDirection(SamplePoint a, SamplePoint b, SamplePoint c);
    static bool AdjacentsChangedDirection(SamplePoint a, SamplePoint b, SamplePoint c);
    static bool ConcaveUp(SamplePoint a, SamplePoint b, SamplePoint c);
    static bool ConcaveDown(SamplePoint a, SamplePoint b, SamplePoint c);
private:
    double X;
    double Y;
    
    bool IsAdjacentTo(SamplePoint other);
};
