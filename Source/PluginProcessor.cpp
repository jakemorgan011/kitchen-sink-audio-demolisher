/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SinkAudioProcessor::SinkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    _constructValueTreeState();
    stutterWidth = ValueTreeState->getRawParameterValue("stutter_width");
    stutterTime = ValueTreeState->getRawParameterValue("stutter_time");
    shiftWidth = ValueTreeState->getRawParameterValue("shift_width");
    shiftAmount = ValueTreeState->getRawParameterValue("shift_amount");
    distortWidth = ValueTreeState->getRawParameterValue("distort_width");
    distortAmount = ValueTreeState->getRawParameterValue("distort_amount");
    dryWet = ValueTreeState->getRawParameterValue("dry_wet");
}

SinkAudioProcessor::~SinkAudioProcessor()
{
}

//==============================================================================
const juce::String SinkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SinkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SinkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SinkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SinkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SinkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SinkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SinkAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SinkAudioProcessor::getProgramName (int index)
{
    return {};
}

void SinkAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SinkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    visualizer.clear();
    threshold.prepareToPlay(sampleRate);
    stutter.prepareToPlay(sampleRate);
}

void SinkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SinkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SinkAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::AudioBuffer<float> bufferCopy;
    bufferCopy.makeCopyOf(buffer);
    bufferCopy.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
    bufferCopy.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
    bufferCopy.applyGain(0.5f);
    visualizer.pushBuffer(bufferCopy);
    
    threshold.processBlock(buffer, stutter);
    
    float height = xyPad.getYValue();
    float bHeight = xyPad.getYValueB();
    float rHeight = xyPad.getYValueR();
    
    threshold.setParameters(*stutterWidth, height, *shiftWidth, rHeight, *distortWidth, bHeight);
    threshold.setRingFreq(*shiftAmount);
    threshold.setAmpFreq(*distortAmount);
    stutter.setParameters(*stutterTime);
    
}

//==============================================================================
bool SinkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SinkAudioProcessor::createEditor()
{
    return new SinkAudioProcessorEditor (*this);
}

//==============================================================================
void SinkAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SinkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SinkAudioProcessor();
}

void SinkAudioProcessor::_constructValueTreeState(){
    ValueTreeState.reset(new juce::AudioProcessorValueTreeState(*this, nullptr, juce::Identifier("sink"),{
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("stutter_width", 1), "stutter_width", juce::NormalisableRange<float>(0.0f,1.f,0.01f), 0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("stutter_time", 1), "stutter_time", juce::NormalisableRange<float>(0.0f,1.f,0.01f), 0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("shift_width",1), "shift_width", juce::NormalisableRange<float>(0.0f,1.f,0.01f), 0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("shift_amount", 1), "shift_amount", juce::NormalisableRange<float>(0.0f,1.f,0.001f), 0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("distort_width",1), "distort_width", juce::NormalisableRange<float>(0.0f,1.f,0.01f), 0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("distort_amount",1), "distort_amount", juce::NormalisableRange<float>(0.f,1.f,0.01f),0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dry_wet",1), "dry_wet", juce::NormalisableRange<float>(0.f,1.f,0.01f), 0.5f)
        
    }));
}
