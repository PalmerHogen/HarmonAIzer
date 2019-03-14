#pragma once

#include "PitchDetector.h"

class ACPitchDetector : public PitchDetector
{
public:
    ACPitchDetector(double sampleRate = 44100.0, int bufferSize = 1024);
    virtual ~ACPitchDetector();
    
    float GetFrequency(SignalR signal);
};
