/*
  ==============================================================================

    stutter.h
    Created: 12 Nov 2024 11:27:24pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "bandpass_filter.h"

class stutter{
public:
    
    stutter();
    ~stutter();
    
    void prepareToPlay(float inSampleRate);
    void processBlock(juce::AudioBuffer<float>& inBuffer);
    void setParameters(float inTime);
    
    
private:
    
    termite::bandpass bandpass;
    
    float sampleRate = 44100.f;
    
    int writehead = 0;
    int readhead = 0;
    
    int max_delay_in_samples = 1500;
    
    float feedbackLeft = 0;
    float feedbackRight = 0;
    
    float feedback_amount = 0.5;
    
    juce::AudioBuffer<float> circularBuffer;
    
    juce::LinearSmoothedValue<float> smoothedStutterTime;
    
};

