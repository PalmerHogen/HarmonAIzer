#include "ACPitchDetector.h"

ACPitchDetector::ACPitchDetector(double sampleRate, int bufferSize): PitchDetector(sampleRate, bufferSize){}

ACPitchDetector::~ACPitchDetector(){}

float ACPitchDetector::GetFrequency(SignalR signal)
{
    SignalC autoCorrelation = Signals::AutoCorrelate(signal, BufferSize);
    float magnitude = 0.0;
    float wavelength = -1.0;
    
    Sample p0, p1, p2;
    p0 = p1 = p2 = 0.0;
    
    for(int n = BufferSize >> 4; n < BufferSize >> 1; n++)
    {
        p0 = p1;
        p1 = p2;
        p2 = autoCorrelation[n].real() / autoCorrelation[2 * n].real();
        
        if(Samples::ChangedDirection(p0, p1, p2) &&
           Samples::PeakFrom(n - 2, p0, p1, p2) > magnitude)
        {
            magnitude = Samples::PeakFrom(n - 2, p0, p1, p2);
            wavelength = Samples::VertexFrom(n - 2, p0, p1, p2);
        }
    }
    delete autoCorrelation;
    return SampleRate / wavelength;
}
