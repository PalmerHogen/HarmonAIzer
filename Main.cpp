#include "MusicalStructure/Key.h"
#include "AI/DynamicProgressionFinder.h"
#include "AI/GreedyProgressionFinder.h"
#include "DSP/ACPitchDetector.h"
#include "DSP/WaveletPitchDetector.h"
#include <iostream>
#include <math.h>

using namespace std;

int main (int argc, char* argv[])
{
    // Several Example Programs
    Chord startingChord = Chord(C, Major);
    Chord endingChord = Chord(A, Minor);
    Key CMajor = Key(C, MajorKey);
    
    int progressionLength = 8;
    
    // ---------------------------------------------------------------------------------------------------------------------------
    cout << endl << "Dynamic Progression Finder:" << endl;
    DynamicProgressionFinder dynamicProgressionFinder;
    dynamicProgressionFinder.SetKey(CMajor);
    Progression dynamicProgression = dynamicProgressionFinder.FindChordProgression(startingChord, endingChord, progressionLength);
    for(Chord chord : dynamicProgression)
        cout << chord.ToString() << endl;
    
    // ---------------------------------------------------------------------------------------------------------------------------
    cout << endl << "Greedy Progression Finder:" << endl;
    GreedyProgressionFinder greedyProgressionFinder;
    greedyProgressionFinder.SetKey(CMajor);
    Progression greedyProgression;
    for(int i = 0; i < progressionLength; i++)
    {
        Chord chord = greedyProgressionFinder.FindNextChord();
        greedyProgression.Add(chord);
        greedyProgressionFinder.CatalogChord(chord);
        cout << chord.ToString() << endl;
    }
    
    // ---------------------------------------------------------------------------------------------------------------------------
    cout << endl << "Greedy Progression Finder with Continuously-updating Key Center:" << endl;
    GreedyProgressionFinder greedyExplorer;
    greedyExplorer.SetKey(CMajor);
    Progression explorativeProgression;
    for(int i = 0; i < progressionLength; i++)
    {
        Chord chord = greedyExplorer.FindNextChord();
        explorativeProgression.Add(chord);
        greedyExplorer.CatalogChord(chord);
        greedyExplorer.SetKey(greedyExplorer.DetectKey());
        cout << chord.ToString() << endl;
    }
    
    // ---------------------------------------------------------------------------------------------------------------------------
    cout << endl << "AutoCorrelation-Based Pitch Detector:" << endl;
    
    int bufferSize = 256;
    double testFrequency = 500;
    cout << "Input Frequency: " << testFrequency << " hz" << endl;
    double sampleRate = 44100.0;
    double signal[bufferSize];
    
    for(int n = 0; n < bufferSize; n++)
    {
        // Sawtooth wave
        signal[n] = Samples::Lerp(-1.0, 1.0, fmod(n * testFrequency / sampleRate , 1.0));
    }
    
    ACPitchDetector pitchDetector(sampleRate, bufferSize);
    
    cout << "Detected Frequency: " << pitchDetector.GetFrequency(signal) <<  " hZ" << endl;
    
    // ---------------------------------------------------------------------------------------------------------------------------
    cout << endl << "Wavelet-Based Pitch Detector:" << endl;
    cout << "Input Frequency: " << testFrequency << " hz" << endl;
    WaveletPitchDetector pitchDetector2(sampleRate, bufferSize);
    
    cout << "Detected Frequency: " << pitchDetector2.GetFrequency(signal) <<  " hZ" << endl;
    
    // ---------------------------------------------------------------------------------------------------------------------------
    return 0;
}
