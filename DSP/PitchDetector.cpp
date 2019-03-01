#include "PitchDetector.h"

PitchDetector::PitchDetector(double sampleRate, int bufferSize)
{
    SampleRate = sampleRate;
    BufferSize = bufferSize;
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

double PitchDetector::GetFrequency(SignalR signal){ return -1.0; }
