#include "PluginProcessor.h"
#include "PluginEditor.h"

HarmonAizerAudioProcessorEditor::HarmonAizerAudioProcessorEditor (HarmonAizerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (500, 200);
    addAndMakeVisible(&SuggestedNote);
    addAndMakeVisible(SuggestedChord);
    SuggestedNote.setSize(100, 100);
    SuggestedChord.setSize(100, 100);
    startTimerHz(60);
}

HarmonAizerAudioProcessorEditor::~HarmonAizerAudioProcessorEditor()
{
}

void HarmonAizerAudioProcessorEditor::timerCallback()
{
    int NoteNumber = 69 + 12 * log2(processor.FrequencyEstimate / 440);
    Note detectedNote = Note(NoteNumber);
    SuggestedNote.setText(detectedNote.ToString(), dontSendNotification);
    Chord suggestion = ChordSuggestor.FindNextChord({detectedNote});
    SuggestedChord.setText(suggestion.ToString(), dontSendNotification);
    ChordSuggestor.CatalogChord(suggestion);
    ChordSuggestor.SetKey(ChordSuggestor.DetectKey());
}

void HarmonAizerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (35.0f);
}

void HarmonAizerAudioProcessorEditor::resized()
{
    SuggestedNote.setBounds(10, 10, getWidth(), getHeight());
    SuggestedChord.setBounds(40, 40, getWidth(), getHeight());
}
