#include "PitchDetector.h"

PitchDetector::PitchDetector(double sampleRate, int bufferSize, int searchDepth)
{
    SampleRate = sampleRate;
    BufferSize = bufferSize;
    SearchDepth = searchDepth;
}

PitchDetector::~PitchDetector(){}

void PitchDetector::SetSampleRate(double sampleRate)
{
    SampleRate = sampleRate;
}

void PitchDetector::SetBufferSize(int bufferSize)
{
    BufferSize = bufferSize;
}

void PitchDetector::SetSearchDepth(int searchDepth)
{
    SearchDepth = searchDepth;
}

double PitchDetector::Downsample(int sample, double* signal, int downSampleFactor)
{
    int factor = downSampleFactor - 1;
    return 0.5 * (signal[(2 << factor) * sample] + signal[(2 << factor) * sample + (1 << factor)]);
}

double PitchDetector::DownsampledWavelength(double* signal, int downsampleFactor)
{
    double wavelength = 1.0;
    
    int signalLength = BufferSize >> downsampleFactor;
    
    if(signalLength < 2)
        return wavelength;
    
    double filtered[signalLength];
    
    double lastPeakSample = 0.0;
    double lastTroughSample = 0.0;
    bool recentlyCrossedZero = false;
    
    SamplePoint p0, p1, p2;
    p1 = SamplePoint(0, filtered[0] = Downsample(0, signal, downsampleFactor));
    p2 = SamplePoint(1, filtered[1] = Downsample(1, signal, downsampleFactor));
    
    for(int n = 2; n < signalLength; n++)
    {
        p0 = p1;
        p1 = p2;
        p2 = SamplePoint(n, filtered[n] = Downsample(n, signal, downsampleFactor));
        
        if(recentlyCrossedZero && SamplePoint::AdjacentsChangedDirection(p0, p1, p2))
        {
            SamplePoint vertex = SamplePoint::VertexOfAdjacent(p0, p1, p2);
            
            if(SamplePoint::ConcaveUp(p0, p1, p2))
            {
                wavelength = 0.5 * (wavelength + vertex.Sample() - lastTroughSample);
                lastTroughSample = vertex.Sample();
            }
            else
            {
                wavelength = 0.5 * (wavelength + vertex.Sample() - lastPeakSample);
                lastPeakSample = vertex.Sample();
            }
            recentlyCrossedZero = false;
        }
        
        if(SamplePoint::CrossedZero(p0, p1))
            recentlyCrossedZero = true;
    } 
    return wavelength;
}

double PitchDetector::GetFrequency(double *signal)
{
    // Uses Haar Wavelet transform to determine the frequency of signal in hZ
    // Returns -1.0 if no frequency could be inferred
    int depth = 0;

    double previousWavelength = -1.0;
  
    while(++depth <= SearchDepth)
    {
        double wavelength = (1 << depth) * DownsampledWavelength(signal, depth);
       
        if(wavelength == previousWavelength)
            return SampleRate / wavelength;
        previousWavelength = wavelength;
    }
    return -1.0;
}

