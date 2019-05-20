#pragma once

#include "PitchDetector.h"

class WaveletPitchDetector : public PitchDetector
{
public:
    WaveletPitchDetector(double sampleRate = 44100.0, int bufferSize = 256, int depthLimit = 6);
    ~WaveletPitchDetector();
    
    float GetFrequency(SignalR signal);
private:
    int DepthLimit;
    float Downsample(int sample, SignalR signal, int downsampleFactor);
    float DownsampledWavelength(SignalR signal, int downsampleFactor);
};
