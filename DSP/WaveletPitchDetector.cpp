#include "WaveletPitchDetector.h"

WaveletPitchDetector::WaveletPitchDetector(double sampleRate, int bufferSize) : PitchDetector(sampleRate, bufferSize){}

WaveletPitchDetector::~WaveletPitchDetector(){}

double WaveletPitchDetector::Downsample(int sample, SignalR signal, int factor)
{
    int mult = (1 << factor);
    int offset = mult / 2;
    return 0.5 * (signal[mult * sample] + signal[mult * sample + offset]);
}

double WaveletPitchDetector::DownsampledWavelength(SignalR signal, int downsampleFactor)
{
    double wavelength = BufferSize * 0.5;
    
    int signalLength = BufferSize >> downsampleFactor;
    
    double filtered[signalLength];
    
    Sample lastPeakSample = 0.0;
    Sample lastTroughSample = 0.0;
    
    Sample p0, p1, p2;
    p0 = p1 = p2 = 0.0;
    
    for(int n = 0; n < signalLength; n++)
    {
        p0 = p1;
        p1 = p2;
        p2 = filtered[n] = Downsample(n, signal, downsampleFactor);
        
        if(Samples::ChangedDirection(p0, p1, p2))
        {
            double vertex = Samples::VertexFrom(n - 2, p0, p1, p2);
            double modeLength = 0.0;
            
            if(Samples::ConcaveUp(p0, p1, p2))
            {
                modeLength = vertex - lastTroughSample;
                lastTroughSample = vertex;
            }
            else
            {
                modeLength = vertex - lastPeakSample;
                lastPeakSample = vertex;
            }

            wavelength = Samples::Lerp(modeLength, wavelength, n / signalLength);
        }
        
    } 
    return wavelength;
}

double WaveletPitchDetector::GetFrequency(SignalR signal)
{
    int depth = 0;

    double previousWavelength = DownsampledWavelength(signal, depth);
  
    while(BufferSize >> ++depth > 2)
    {
        double wavelength = (1 << depth) * DownsampledWavelength(signal, depth);
        double deltaWavelength = abs(wavelength - previousWavelength);
        
        if(deltaWavelength < Cent50Drift * log2(wavelength))
            return SampleRate / wavelength;
        
        previousWavelength = wavelength;
    }
    return -1.0;
}
