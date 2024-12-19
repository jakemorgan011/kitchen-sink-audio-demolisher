/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "customLookAndFeel.h"

//==============================================================================
/**
*/
class SinkAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SinkAudioProcessorEditor (SinkAudioProcessor&);
    ~SinkAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    customLookAndFeel customLookAndFeel;
    class customLookAndFeel widthLookAndFeelG;
    class customLookAndFeel widthLookAndFeelR;
    class customLookAndFeel widthLookAndFeelB;
    
    juce::Slider stutterWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stutterWidthSliderAttachment;
    
    juce::Slider stutterTimeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stutterTimeSliderAttachment;
    
    juce::Slider shiftWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> shiftWidthSliderAttachment;
    
    juce::Slider shiftAmountSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> shiftAmountSliderAttachment;
    
    juce::Slider distortWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distortWidthSliderAttachment;
    
    juce::Slider distortAmountSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distortAmountSliderAttachment;
    
    juce::Slider dryWetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetSliderAttachment;
    
    juce::Label widthLabel;
    juce::Label modLabel;
    juce::Label dryWetLabel;
    
    // idek
    /// TODO: be able to access the vts from the xypad.
    /// how am i going to do that.
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SinkAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SinkAudioProcessorEditor)
};
