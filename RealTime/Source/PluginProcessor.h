#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MusicalStructure/Note.h"
#include "MusicalStructure/Chord.h"
#include "AI/GreedyProgressionFinder.h"
#include "DSP/WaveletPitchDetector.h"
#include <unordered_set>
#include <queue>

using namespace std;

class HarmonAizerAudioProcessor  : public AudioProcessor
{
public:
    HarmonAizerAudioProcessor();
    ~HarmonAizerAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    GreedyProgressionFinder ProgressionPlanner;
    uint8 VelocityThreshold = 0;
    uint8 BassChoiceOctave = 3;
    uint8 ChoiceOctave = 5;
private:
    Chord StoredChord;
    unordered_set<int> StoredNoteNumbers;
    unordered_map<Note, int> ActiveNotes;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonAizerAudioProcessor)
};
