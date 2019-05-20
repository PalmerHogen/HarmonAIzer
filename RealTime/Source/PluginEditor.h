#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MusicalStructure/Key.h"
#include "MusicalStructure/Note.h"
#include "Parameters/UserParameters.h"
#include "PluginProcessor.h"

class HarmonAizerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    HarmonAizerAudioProcessorEditor (HarmonAizerAudioProcessor&);
    ~HarmonAizerAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;
private:
    void RegenerateAllChords();
    HarmonAizerAudioProcessor& processor;
    Note KeyAxis = Note(C);
    KeyType TypeOfKey = MajorKey;
    ComboBox KeyAxisSelector;
    ComboBox KeyTypeSelector;
    Slider VelocityThresholdSlider;
    Slider OutOfKeyPenaltySlider;
    Slider NonChordTonePenaltySlider;
    Slider SecondaryDominantPenaltySlider;
    Slider ModalMixturePenaltySlider;
    Slider PedalTonePenaltySlider;
    Slider RepeatedChordPenaltySlider;
    
    Slider InnerVoiceOctaveSlider {"Inner-Voice Octave"};
    Slider BassOctaveSlider {"Bass Octave"};
    
    ToggleButton Dom7Toggle {"Using Dominant Sevenths"};
    ToggleButton Maj7Toggle {"Using Major Sevenths"};
    ToggleButton Min7Toggle {"Using Minor Sevenths"};
    ToggleButton AugmentedToggle {"Using Augmented Chords"};
    ToggleButton DiminishedToggle {"Using Diminished Chords"};
    ToggleButton Dim7Toggle{"Using Diminished Sevenths"};
    ToggleButton HalfDim7Toggle {"Using Half-Diminished Sevenths"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonAizerAudioProcessorEditor)
};
