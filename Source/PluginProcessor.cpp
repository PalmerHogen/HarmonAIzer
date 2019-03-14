#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <unordered_map>
#include "MusicalStructure/Chord.h"
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
    PitchDetector.SetSampleRate(sampleRate);
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
    int numSamples = buffer.getNumSamples();
    float* leftChannel = buffer.getWritePointer(0);
    PitchDetector.SetBufferSize(numSamples);
    if (FloatVectorOperations::findMaximum(leftChannel, numSamples) < 0.05f)
        return;
    FrequencyEstimate = PitchDetector.GetFrequency(leftChannel);
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

int HarmonAizerAudioProcessor::FrequencyToMidi(float frequency)
{
    return 69 + 12 * log2(frequency * PitchAdjust);
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonAizerAudioProcessor();
}
