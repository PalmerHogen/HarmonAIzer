#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class HarmonAizerAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    HarmonAizerAudioProcessorEditor (HarmonAizerAudioProcessor&);
    ~HarmonAizerAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
private:
    HarmonAizerAudioProcessor& processor;
    GreedyProgressionFinder ChordSuggestor;
    
    Label SuggestedNote;
    Label SuggestedChord;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonAizerAudioProcessorEditor)
};
