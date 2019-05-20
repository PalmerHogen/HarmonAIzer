#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <unordered_map>
#include <math.h>
#include <iostream>

HarmonAizerAudioProcessor::HarmonAizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{ 
}

HarmonAizerAudioProcessor::~HarmonAizerAudioProcessor()
{
}

const String HarmonAizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmonAizerAudioProcessor::acceptsMidi() const
{
    return true;
}

bool HarmonAizerAudioProcessor::producesMidi() const
{
    return true;
}

bool HarmonAizerAudioProcessor::isMidiEffect() const
{
    return true;
}

double HarmonAizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmonAizerAudioProcessor::getNumPrograms()
{
    return 1;
}

int HarmonAizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmonAizerAudioProcessor::setCurrentProgram (int index)
{
    
}

const String HarmonAizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void HarmonAizerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void HarmonAizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void HarmonAizerAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmonAizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HarmonAizerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    MidiBuffer::Iterator iterator(midiMessages);
    MidiMessage msg;
    bool noteOnWasEmphasized = false;
    int samplePosition;
    
    uint8 velocity = 0;
    
    jassert (buffer.getNumChannels() == 0);
    
    for (MidiBuffer::Iterator it (midiMessages); it.getNextEvent (msg, samplePosition);)
    {
        if      (msg.isNoteOn())
        {
            ActiveNotes[Note(msg.getNoteNumber())] = samplePosition;
            noteOnWasEmphasized |= msg.getVelocity() > VelocityThreshold;
            velocity = msg.getVelocity();
        }
        else if (msg.isNoteOff())
        {
            ActiveNotes.erase(Note(msg.getNoteNumber()));
        }
    }
    
    midiMessages.clear();
    
    if(noteOnWasEmphasized)
    {
        Chord newChord = ProgressionPlanner.FindNextChord(StoredChord, ActiveNotes);
        if(newChord != StoredChord)
        {
            for(auto noteNumber : StoredNoteNumbers)
            {
                midiMessages.addEvent(MidiMessage::noteOff(1, noteNumber, (uint8)0), samplePosition);
            }
            StoredNoteNumbers.clear();
            StoredChord = newChord;
            StoredNoteNumbers.insert(StoredChord.Root.GetId() + 12 * BassChoiceOctave);
            midiMessages.addEvent(MidiMessage::noteOn(1, StoredChord.Root.GetId() + 12 * BassChoiceOctave, velocity), samplePosition);
            for(auto note : StoredChord.Notes)
            {
                StoredNoteNumbers.insert(note.GetId() + 12 * ChoiceOctave);
                midiMessages.addEvent(MidiMessage::noteOn(1, note.GetId() + 12 * ChoiceOctave, velocity), samplePosition);
            }
            ProgressionPlanner.CatalogChord(newChord);
        }
    }
}

bool HarmonAizerAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* HarmonAizerAudioProcessor::createEditor()
{
    return new HarmonAizerAudioProcessorEditor (*this);
}

void HarmonAizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    
}

void HarmonAizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonAizerAudioProcessor();
}
