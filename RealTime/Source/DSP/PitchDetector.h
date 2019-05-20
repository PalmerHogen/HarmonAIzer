#pragma once

#include "../MusicalStructure/Note.h"
#include "Signal.h"

const float Cent100Drift = log2(12);
const float Cent50Drift = log2(24);

class PitchDetector
{
public:
    PitchDetector(double sampleRate = 44100.0, int bufferSize = 1024);
    virtual ~PitchDetector();
    
    void SetSampleRate(double sampleRate);
    void SetBufferSize(int bufferSize);
    virtual float GetFrequency(SignalR signal);
protected:
    double SampleRate;
    int BufferSize;
};
