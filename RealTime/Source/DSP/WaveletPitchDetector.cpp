#include "WaveletPitchDetector.h"

WaveletPitchDetector::WaveletPitchDetector(double sampleRate, int bufferSize, int depthLimit) : PitchDetector(sampleRate, bufferSize)
{
    DepthLimit = depthLimit;
}

WaveletPitchDetector::~WaveletPitchDetector(){}

float WaveletPitchDetector::Downsample(int sample, SignalR signal, int factor)
{
    int mult = 1 << factor;
    int offset = mult >> 1;
    return 0.5f * (signal[mult * sample] + signal[mult * sample + offset]);
}

float WaveletPitchDetector::DownsampledWavelength(SignalR signal, int downsampleFactor)
{
    float wavelength = -1.0f;
    bool initialized = false;
    int signalLength = BufferSize >> downsampleFactor;
    
    Sample lastPeakSample;
    Sample lastTroughSample;
    Sample p0, p1, p2;
    lastPeakSample = lastTroughSample = p0 = p1 = p2 = 0.0;
    
    bool recentlyCrossedZero = false;
    
    for(int n = 0; n < signalLength; n++)
    {
        p0 = p1;
        p1 = p2;
        p2 = Downsample(n, signal, downsampleFactor);
        
        if(recentlyCrossedZero && Samples::ChangedDirection(p0, p1, p2))
        {
            Sample vertex = Samples::VertexFrom(n - 2, p0, p1, p2);
            
            bool concavity = Samples::ConcaveUp(p0, p1, p2);
            float modeLength = concavity * (vertex - lastTroughSample) + !concavity * (vertex - lastPeakSample);
            
            lastTroughSample = concavity * vertex + !concavity * lastTroughSample;
            lastPeakSample = !concavity * vertex + concavity * lastPeakSample;
            
            wavelength = !initialized * modeLength + initialized * (0.75f * wavelength + 0.25f * modeLength);
            
            initialized = true;
            recentlyCrossedZero = false;
        }
        else
        {
            recentlyCrossedZero |= Samples::CrossedZero(p1, p2);
        }
        
    } 
    return wavelength;
}

float WaveletPitchDetector::GetFrequency(SignalR signal)
{
    int depth = 0;

    float previousWavelength = DownsampledWavelength(signal, depth);
  
    while(BufferSize >> ++depth > 2 && depth <= DepthLimit)
    {
        float wavelength = (1 << depth) * DownsampledWavelength(signal, depth);
        float deltaWavelength = abs(wavelength - previousWavelength);
        
        if(deltaWavelength < Cent50Drift * log2(wavelength))
            return SampleRate / wavelength;
        
        previousWavelength = wavelength;
    }
    return -1.0f;
}
