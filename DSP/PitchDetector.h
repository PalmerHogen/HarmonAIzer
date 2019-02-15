#pragma once

#include "../MusicalStructure/Note.h"
#include "SamplePoint.h"

class PitchDetector
{
public:
    PitchDetector(double sampleRate = 44100.0, int bufferSize = 1024, int searchDepth = 8);
    ~PitchDetector();
    
    void SetSampleRate(double sampleRate);
    void SetBufferSize(int bufferSize);
    void SetSearchDepth(int searchDepth);
    double GetFrequency(double* signal);
private:
    double SampleRate;
    int BufferSize;
    int SearchDepth;
    
    double Downsample(int sample, double* signal, int downsampleFactor);
    double DownsampledWavelength(double* signal, int downsampleFactor);
};
