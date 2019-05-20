#include "PluginProcessor.h"
#include "PluginEditor.h"

HarmonAizerAudioProcessorEditor::HarmonAizerAudioProcessorEditor (HarmonAizerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (500, 300);
    
    // Changing Key of Harmonizer----------------------------------------
    KeyAxisSelector.setEditableText(false);
    KeyAxisSelector.addItem("None", 0);
    for(int keyAxis = C; keyAxis <= B; keyAxis++)
    {
        KeyAxisSelector.addItem (Note(keyAxis).ToString(), keyAxis + 1);
    }
    KeyAxisSelector.onChange = [this]
        {
            if(KeyAxisSelector.getSelectedId() == 0)
            {
                processor.ProgressionPlanner.UnsetKey();
            }
            else
            {
                KeyAxis = Note(KeyAxisSelector.getSelectedId() - 1);
                processor.ProgressionPlanner.SetKey(Key(KeyAxis, TypeOfKey));
            }
        };
    KeyAxisSelector.setSelectedId(1);
    addAndMakeVisible(&KeyAxisSelector);
    
    KeyTypeSelector.setEditableText(false);
    KeyTypeSelector.addItem("None", 0);
    KeyTypeSelector.addItem("Natural Minor", (int) NaturalMinorKey + 1);
    KeyTypeSelector.addItem("Harmonic Minor", (int) HarmonicMinorKey + 1);
    KeyTypeSelector.addItem("Major", (int) MajorKey + 1);
    KeyTypeSelector.onChange = [this]
        {
            if(KeyTypeSelector.getSelectedId() == 0)
            {
                processor.ProgressionPlanner.UnsetKey();
            }
            else
            {
                TypeOfKey = (KeyType)(KeyTypeSelector.getSelectedId() - 1);
                processor.ProgressionPlanner.SetKey(Key(KeyAxis, TypeOfKey));
            }
        };
    KeyTypeSelector.setSelectedId(3);
    addAndMakeVisible(&KeyTypeSelector);
    // ------------------------------------------------------------------
    
    VelocityThresholdSlider.setRange(0, 127, 1);
    VelocityThresholdSlider.onValueChange = [this]
        {
            processor.VelocityThreshold = VelocityThresholdSlider.getValue();
        };
    VelocityThresholdSlider.setValue(0.0);
    addAndMakeVisible(&VelocityThresholdSlider);
    // ------------------------------------------------------------------
    OutOfKeyPenaltySlider.setRange(0, 10, 1);
    OutOfKeyPenaltySlider.onValueChange = [&]
        {processor.ProgressionPlanner.OutOfKeyPenalty = OutOfKeyPenaltySlider.getValue();};
    OutOfKeyPenaltySlider.setValue(OutOfKeyPenalty);
    addAndMakeVisible(&OutOfKeyPenaltySlider);
    
    NonChordTonePenaltySlider.setRange(0, 10, 1);
    NonChordTonePenaltySlider.onValueChange = [&]
        {processor.ProgressionPlanner.NonChordTonePenalty = NonChordTonePenaltySlider.getValue();};
    NonChordTonePenaltySlider.setValue(NonChordTonePenalty);
    addAndMakeVisible(&NonChordTonePenaltySlider);
    
    SecondaryDominantPenaltySlider.setRange(0, 10, 1);
    SecondaryDominantPenaltySlider.onValueChange = [&]
        {processor.ProgressionPlanner.SecondaryDominantPenalty = SecondaryDominantPenaltySlider.getValue();};
    SecondaryDominantPenaltySlider.setValue(SecondaryDominantPenalty);
    addAndMakeVisible(&SecondaryDominantPenaltySlider);
    
    ModalMixturePenaltySlider.setRange(0, 10, 1);
    ModalMixturePenaltySlider.onValueChange = [&]
        {processor.ProgressionPlanner.ModalMixturePenalty = ModalMixturePenaltySlider.getValue();};
    ModalMixturePenaltySlider.setValue(ModalMixturePenalty);
    addAndMakeVisible(&ModalMixturePenaltySlider);
    
    PedalTonePenaltySlider.setRange(0, 10, 1);
    PedalTonePenaltySlider.onValueChange = [&]
        {processor.ProgressionPlanner.PedalTonePenalty = PedalTonePenaltySlider.getValue();};
    PedalTonePenaltySlider.setValue(PedalTonePenalty);
    addAndMakeVisible(&PedalTonePenaltySlider);
    
    RepeatedChordPenaltySlider.setRange(0, 10, 1);
    RepeatedChordPenaltySlider.onValueChange = [&]
        {processor.ProgressionPlanner.RepeatedChordPenalty = RepeatedChordPenaltySlider.getValue();};
    RepeatedChordPenaltySlider.setValue(RepeatedChordPenalty);
    addAndMakeVisible(&RepeatedChordPenaltySlider);
    
    //------------------------------------------------------------------
    Dom7Toggle.onStateChange = [this]
        {
            processor.ProgressionPlanner.UseDominantSevenths = Dom7Toggle.getToggleState();
            processor.ProgressionPlanner.GenerateAllChords();
        };
    addAndMakeVisible(&Dom7Toggle);
    
    Maj7Toggle.onStateChange = [this]
        {
            processor.ProgressionPlanner.UseMajorSevenths = Maj7Toggle.getToggleState();
            processor.ProgressionPlanner.GenerateAllChords();
        };
    addAndMakeVisible(&Maj7Toggle);
    
    Min7Toggle.onStateChange = [this]
        {
            processor.ProgressionPlanner.UseMinorSevenths = Min7Toggle.getToggleState();
            processor.ProgressionPlanner.GenerateAllChords();
        };
    addAndMakeVisible(&Min7Toggle);
    
    AugmentedToggle.onStateChange = [this]
        {
            processor.ProgressionPlanner.UseAugmentedChords = AugmentedToggle.getToggleState();
            processor.ProgressionPlanner.GenerateAllChords();
        };
    addAndMakeVisible(&AugmentedToggle);
    
    DiminishedToggle.onStateChange = [this]
        {
            bool toggleState = DiminishedToggle.getToggleState();
            if(!toggleState)
            {
                Dim7Toggle.setToggleState(toggleState, juce::NotificationType::sendNotification);
                HalfDim7Toggle.setToggleState(toggleState, juce::NotificationType::sendNotification);
            }
            processor.ProgressionPlanner.UseDiminishedChords = toggleState;
            processor.ProgressionPlanner.GenerateAllChords();
        };
    addAndMakeVisible(&DiminishedToggle);
    
    Dim7Toggle.onStateChange = [this]
        {
            bool toggleState = Dim7Toggle.getToggleState();
            if(toggleState && !DiminishedToggle.getToggleState())
                DiminishedToggle.setToggleState(toggleState, juce::NotificationType::sendNotification);
            processor.ProgressionPlanner.UseDiminishedSevenths = toggleState;
            processor.ProgressionPlanner.GenerateAllChords();
        };
    addAndMakeVisible(&Dim7Toggle);
    
    HalfDim7Toggle.onStateChange = [this]
    {
        bool toggleState = HalfDim7Toggle.getToggleState();
        if(toggleState && !DiminishedToggle.getToggleState())
            DiminishedToggle.setToggleState(toggleState, juce::NotificationType::sendNotification);
        processor.ProgressionPlanner.UseHalfDiminishedSevenths = toggleState;
        processor.ProgressionPlanner.GenerateAllChords();
    };
    addAndMakeVisible(&HalfDim7Toggle);
    
    Maj7Toggle.setToggleState(true, sendNotification);
    Min7Toggle.setToggleState(true, sendNotification);
    Dom7Toggle.setToggleState(true, sendNotification);
    AugmentedToggle.setToggleState(true, sendNotification);
    DiminishedToggle.setToggleState(true, sendNotification);
    Dim7Toggle.setToggleState(true, sendNotification);
    HalfDim7Toggle.setToggleState(true, sendNotification);
    
    // ----------------------------------------------------------------
    BassOctaveSlider.setRange(0, 4, 1);
    BassOctaveSlider.onValueChange = [this]
        {
            processor.BassChoiceOctave = BassOctaveSlider.getValue();
        };
    BassOctaveSlider.setValue(processor.BassChoiceOctave);
    addAndMakeVisible(&BassOctaveSlider);
    
    InnerVoiceOctaveSlider.setRange(1, 7, 1);
    InnerVoiceOctaveSlider.onValueChange = [this]
        {
            processor.ChoiceOctave = InnerVoiceOctaveSlider.getValue();
        };
    InnerVoiceOctaveSlider.setValue(processor.ChoiceOctave);
    addAndMakeVisible(&InnerVoiceOctaveSlider);
}

HarmonAizerAudioProcessorEditor::~HarmonAizerAudioProcessorEditor()
{
}

void HarmonAizerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Velocity Threshold", 0, 25, getWidth(), 25, juce::Justification::centred, 1);
    g.drawFittedText("Out-of-Key Penalty", 0, 70, getWidth() / 2, 20, juce::Justification::centred, 1);
    g.drawFittedText("Non-Chord-Tone Penalty", getWidth() / 2, 70, getWidth() / 2, 20, juce::Justification::centred, 1);
    g.drawFittedText("Secondary Dominant Penalty", 0, 110, getWidth() / 2, 20, juce::Justification::centred, 1);
    g.drawFittedText("Modal Mixture Penalty", getWidth() / 2, 110, getWidth() / 2, 20, juce::Justification::centred, 1);
    g.drawFittedText("Pedal-Tone Penalty", 0, 150, getWidth() / 2, 20, juce::Justification::centred, 1);
    g.drawFittedText("Repeated-Chord Penalty", getWidth() / 2, 150, getWidth() / 2, 20, juce::Justification::centred, 1);
}

void HarmonAizerAudioProcessorEditor::resized()
{
    KeyAxisSelector.setBounds(0, 0, getWidth() / 2, 20);
    KeyTypeSelector.setBounds(getWidth() / 2, 0, getWidth() / 2, 20);
    VelocityThresholdSlider.setBounds(0, 50, getWidth(), 20);
    
    OutOfKeyPenaltySlider.setBounds(0, 90, getWidth() / 2, 20);
    NonChordTonePenaltySlider.setBounds(getWidth() / 2, 90, getWidth() / 2, 20);
    SecondaryDominantPenaltySlider.setBounds(0, 130, getWidth() / 2, 20);
    ModalMixturePenaltySlider.setBounds(getWidth() / 2, 130, getWidth() / 2, 20);
    PedalTonePenaltySlider.setBounds(0, 170, getWidth() / 2, 20);
    RepeatedChordPenaltySlider.setBounds(getWidth() / 2, 170, getWidth() / 2, 20);
    
    BassOctaveSlider.setBounds(0, 195, getWidth() / 2, 20);
    InnerVoiceOctaveSlider.setBounds(getWidth() / 2, 195, getWidth() / 2, 20);
    
    Dom7Toggle.setBounds(getWidth() / 2, 220, getWidth() / 2, 20);
    Maj7Toggle.setBounds(0, 240, getWidth() / 2, 20);
    Min7Toggle.setBounds(getWidth() / 2, 240, getWidth() / 2, 20);
    AugmentedToggle.setBounds(0, 260, getWidth() / 2, 20);
    DiminishedToggle.setBounds(getWidth() / 2, 260, getWidth() / 2, 20);
    Dim7Toggle.setBounds(0, 280, getWidth() / 2, 20);
    HalfDim7Toggle.setBounds(getWidth() / 2, 280, getWidth() / 2, 20);
}
