/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "xyPad.h"
#include "threshold_total.h"
//==============================================================================
/**
*/
class SinkAudioProcessor  : public juce::AudioProcessor
{
public:
    
    class visualizer : public juce::AudioVisualiserComponent {
    public:
        visualizer() : juce::AudioVisualiserComponent(1){
            setBufferSize(256);
            setSamplesPerBlock(32);
            setColours(juce::Colours::lightgrey, juce::Colours::whitesmoke);
        }
    };
    visualizer visualizer;
    
    xyPad xyPad;
    //==============================================================================
    SinkAudioProcessor();
    ~SinkAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState& getVTS(){
        return *ValueTreeState;
    }

private:
    
    stutter stutter;
    threshold threshold;
    
    std::atomic<float>* stutterWidth = nullptr;
    std::atomic<float>* stutterTime = nullptr;
    std::atomic<float>* shiftWidth = nullptr;
    std::atomic<float>* shiftAmount = nullptr;
    std::atomic<float>* distortWidth = nullptr;
    std::atomic<float>* distortAmount = nullptr;
    std::atomic<float>* dryWet = nullptr;
    
    juce::LinearSmoothedValue<float> smoothedStutterWidth;
    juce::LinearSmoothedValue<float> smoothedStutterTime;
    juce::LinearSmoothedValue<float> smoothedShiftWidth;
    juce::LinearSmoothedValue<float> smoothedShiftAmount;
    juce::LinearSmoothedValue<float> smoothedDistortWidth;
    juce::LinearSmoothedValue<float> smoothedDistortAmount;
    juce::LinearSmoothedValue<float> smoothedDryWet;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState> ValueTreeState;
    
    void _constructValueTreeState();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SinkAudioProcessor)
};
