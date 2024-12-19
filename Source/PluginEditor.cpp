/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SinkAudioProcessorEditor::SinkAudioProcessorEditor (SinkAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(audioProcessor.visualizer);
    addAndMakeVisible(audioProcessor.xyPad);
    
    widthLookAndFeelB.colourSelect = juce::Colours::blue;
    widthLookAndFeelG.colourSelect = juce::Colours::lime;
    widthLookAndFeelR.colourSelect = juce::Colours::red;
    // everything is normalized.
    stutterWidthSlider.setLookAndFeel(&widthLookAndFeelB);
    stutterWidthSlider.setRange(0.f,1.0f);
    stutterWidthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    stutterWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0,0);
    addAndMakeVisible(stutterWidthSlider);
    stutterWidthSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"stutter_width", stutterWidthSlider));
    
    stutterTimeSlider.setRange(0.f,1.0f);
    stutterTimeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    stutterTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(stutterTimeSlider);
    stutterTimeSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"stutter_time", stutterTimeSlider));
    
    shiftWidthSlider.setLookAndFeel(&widthLookAndFeelR);
    shiftWidthSlider.setRange(0.f,1.0f);
    shiftWidthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    shiftWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(shiftWidthSlider);
    shiftWidthSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"shift_width",shiftWidthSlider));
    
    shiftAmountSlider.setRange(0.f,1.f);
    shiftAmountSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    shiftAmountSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0,0);
    addAndMakeVisible(shiftAmountSlider);
    shiftAmountSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(), "shift_amount", shiftAmountSlider));
    
    distortWidthSlider.setLookAndFeel(&widthLookAndFeelG);
    distortWidthSlider.setRange(0.f,1.0f);
    distortWidthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    distortWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(distortWidthSlider);
    distortWidthSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"distort_width", distortWidthSlider));
    
    distortAmountSlider.setRange(0.f,1.0f);
    distortAmountSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    distortAmountSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(distortAmountSlider);
    distortAmountSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"distort_amount", distortAmountSlider));
    
    dryWetSlider.setRange(0.f,1.0f);
    dryWetSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryWetSlider.setTextBoxStyle(juce::Slider::NoTextBox,true,0,0);
    addAndMakeVisible(dryWetSlider);
    dryWetSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"dry_wet", dryWetSlider));
    
    widthLabel.setText("Widths", juce::dontSendNotification);
    addAndMakeVisible(widthLabel);
    widthLabel.setColour(1, juce::Colours::lime);
    
    modLabel.setText("fx Mod", juce::dontSendNotification);
    addAndMakeVisible(modLabel);
    
    dryWetLabel.setText("Dry/Wet", juce::dontSendNotification);
    addAndMakeVisible(dryWetLabel);
    
    setSize (500, 400);
}

SinkAudioProcessorEditor::~SinkAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void SinkAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey);
}

void SinkAudioProcessorEditor::resized()
{
    auto kBounds = getBounds();
    kBounds.removeFromBottom(300);
    
    stutterWidthSlider.setBounds(0,0,100,100);
    stutterTimeSlider.setBounds(60, 100, 50, 50);
    
    shiftWidthSlider.setBounds(120,0,100,100);
    shiftAmountSlider.setBounds(180, 100, 50, 50);
    
    distortWidthSlider.setBounds(240,0,100,100);
    distortAmountSlider.setBounds(300, 100, 50, 50);
    dryWetSlider.setBounds(380,0,75,75);
    
    widthLabel.setBounds(400,150,50,25);
    modLabel.setBounds(400, 200, 50, 25);
    dryWetLabel.setBounds(380,75,50,25);
    
    
    // lol i made the signal mono but I don't know how to change the buffer to mono so I stretch the second signal off the screen
    // it is really stupid sorry.
    audioProcessor.visualizer.setBounds(0,150,500,250);
    
    audioProcessor.xyPad.setBounds(0, 150, 500, 250);
}
