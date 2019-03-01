#pragma once

#include "PitchDetector.h"

class WaveletPitchDetector : public PitchDetector
{
public:
    WaveletPitchDetector(double sampleRate = 44100.0, int bufferSize = 1024);
    ~WaveletPitchDetector();
    
    double GetFrequency(SignalR signal);
private:
    double Downsample(int sample, SignalR signal, int downsampleFactor);
    double DownsampledWavelength(SignalR signal, int downsampleFactor);
};
